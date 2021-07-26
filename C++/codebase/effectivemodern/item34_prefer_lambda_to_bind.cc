#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <string>

#include <boost/noncopyable.hpp>

#include "spdlog/spdlog.h"

using Time = std::chrono::steady_clock::time_point;
using Duration = std::chrono::steady_clock::duration;
enum class Sound { Beep, Siren, Whistle };
enum class Volume { Normal, Loud, LoudPlusPlus };
auto now = []() -> auto { return std::chrono::steady_clock::now(); };

class Widget {
public:
    void setStr(const std::string& str) {
        str_ = str;
    }

    std::string str() const {
        return str_;
    }

private:
    std::string str_;
};

void setWidget(Widget& widget, const std::string& str) {
    widget.setStr(str);
}

// 在 time 时间点，以 sound 响 duration
void setAlarm(Time time, Sound sound, Duration duration) {
    std::this_thread::sleep_until(time);
    spdlog::info("闹钟响了：{} {} {} ...", sound, sound, sound);
    std::this_thread::sleep_for(duration);
    spdlog::info("闹钟停止响");
}

void setAlarm(Time time, Sound sound, Duration duration, Volume volume) {
    std::this_thread::sleep_until(time);
    spdlog::info("闹钟响了：{} {} ...", sound, volume);
    std::this_thread::sleep_for(duration);
    spdlog::info("闹钟停止响");
}

class Bar : public boost::noncopyable {

};

int func(int i , Widget& w, Bar& b, const std::string& str) {
    w.setStr(str);
    return i;
}

// int func(int i , Widget& w, const std::string& str) {
//     w.setStr(str);
//     return i;
// }

template <typename Func, typename... Args>
void scheduleB(Func&& func, Args&&... args) {
    auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    task();
}


template <typename Func, typename... Args>
void scheduleL(Func&& func, Args&&... args) {
    auto task = [func, args...]() mutable {
        std::invoke(func, std::forward<Args>(args)...);
    };
    task();
}


template <typename Func, typename... Args>
void scheduleL2(Func&& func, Args&&... args) {
    auto task = [func, &args...]() mutable {
        std::invoke(func, std::forward<Args>(args)...);
    };
    task();
}

// https://www.coder.work/article/576939
// template <typename Func, typename... Args>
// void scheduleL3(Func&& func, Args&&... args) {
//     auto task = [func = std::move(func), ... args = std::forward<Args>(args)]() mutable {
//         func(args...);
//     };
//     task();
// }

int main() {
    {  // 现在设定 time 为 2s 后，duration 为 1s，只有 Sound 为参数

        using namespace std::chrono_literals;
        using namespace std::placeholders;

#if 0 // ------------------ lambda 比 std::bind 更加直观 ------------------

    auto setSoundL = [](Sound sound){
        setAlarm(now() + 2s, sound, std::chrono::seconds(1));
    };
    auto setSoundB = std::bind(setAlarm, now() + 2s, _1, 1s);

    // [2021-07-17 09:08:34.561] [info] 闹钟响了：0 0 0 ...
    // [2021-07-17 09:08:35.562] [info] 闹钟停止响
    // [2021-07-17 09:08:35.562] [info] 闹钟响了：1 1 1 ...
    // [2021-07-17 09:08:36.562] [info] 闹钟停止响
    // setSoundL(Sound::Beep);
    // setSoundB(Sound::Siren);

    // [2021-07-17 09:10:28.429] [info] 闹钟响了：1 1 1 ...
    // [2021-07-17 09:10:29.430] [info] 闹钟停止响
    // [2021-07-17 09:10:31.430] [info] 闹钟响了：0 0 0 ...
    // [2021-07-17 09:10:32.431] [info] 闹钟停止响
    // setSoundB(Sound::Siren);
    // setSoundL(Sound::Beep);
    
    auto setSoundB2 = std::bind(setAlarm, std::bind(std::plus<>(), std::bind(now), 2s), _1, 1s);

    // [2021-07-17 09:56:10.054] [info] 闹钟响了：0 0 0 ...
    // [2021-07-17 09:56:11.055] [info] 闹钟停止响
    // [2021-07-17 09:56:11.055] [info] 闹钟响了：2 2 2 ...
    // [2021-07-17 09:56:12.055] [info] 闹钟停止响
    // [2021-07-17 09:56:14.056] [info] 闹钟响了：1 1 1 ...
    // [2021-07-17 09:56:15.056] [info] 闹钟停止响
    std::this_thread::sleep_for(2s);
    setSoundL(Sound::Beep);
    setSoundB(Sound::Whistle);
    setSoundB2(Sound::Siren);
#endif

#if 0 // ------------------ 面对重载时 lambda 比 std::bind 有更强的适应性

    // ------------------ 增加重载函数 void setAlarm(Time time, Sound sound,
    // Duration duration, Volume volume) lambda 保持不变，不会报错
    auto setSoundL = [](Sound sound) {
        setAlarm(now() + 2s, sound, std::chrono::seconds(1));
    };
    // error: no instance of overloaded function "std::bind" matches the argument
    // list -- argument types are: (<unknown-type>,
    // std::_Bind<std::plus<void> (std::_Bind<type ()>,
    // std::chrono::duration<int64_t, std::ratio<1L, 1L>>)>, const
    // std::_Placeholder<1>, std::chrono::duration<int64_t, std::ratio<1L,
    // 1L>>)
    // auto setSoundB = std::bind(
    //     setAlarm, std::bind(std::plus<>(), std::bind(now), 2s), _1, 1s);

    using SetAlarm3ParaType = void(*)(Time, Sound, Duration);

    auto setSoundB = std::bind(
        static_cast<SetAlarm3ParaType>(setAlarm),
        std::bind(std::plus<>(), std::bind(now), 2s),
        _1,
        1s
    );

    setSoundL(Sound::Beep);
    setSoundB(Sound::Siren);
#endif

#if 0 // ------------------ 表达相同的简单运算时 lambda 比 std::bind 更简单

    int left = 0;
    int right = 10;
    auto betweenL = [left, right](int x) -> bool {
        return x >= left && x <= right;
    };

    auto betweenB = std::bind(
        std::logical_and<>(),
        std::bind(std::less_equal<>(), left, _1),
        std::bind(std::less_equal<>(), _1, right)
    );

    // [2021-07-17 10:20:48.469] [info] true, true
    // [2021-07-17 10:20:48.469] [info] false, false
    spdlog::info("{}, {}", betweenL(1), betweenB(1));
    spdlog::info("{}, {}", betweenL(11), betweenB(11));
#endif


#if 0 // ------------------ lambda 更清晰地表达：参数是按值捕获还是按引用捕获？是按值传递还是按引用传递？
    
    Widget w;
    w.setStr("yue");

    auto setWidgetL = [&w] (const std::string& str) { // 按引用捕获
        setWidget(w, str);
    };

    auto setWidgetB = std::bind( // bind 会拷贝实参，所以是按值捕获，bind 通过完美转发传递参数，所以是按引用传递
        setWidget,
        w,
        _1
    );
    setWidgetL("rui");
    spdlog::info("{}", w.str()); // [2021-07-17 10:45:34.549] [info] rui
    setWidgetB("feng");
    spdlog::info("{}", w.str()); // [2021-07-17 10:45:34.549] [info] rui

#endif

#if 1
    Widget w;
    w.setStr("yue");

    // Bar b;

    // func(5, w, b, "rui");
    // spdlog::info("{}", w.str()); // [2021-07-17 14:45:48.622] [info] rui

    // func(5, w, "rui");
    // spdlog::info("{}", w.str()); // [2021-07-17 14:44:35.221] [info] rui


    // scheduleB(func, 5, w, b, "rui"); // no matching function for call to
    // scheduleB(func, 5, w, std::ref(b), "rui");
    // spdlog::info("{}", w.str()); // [2021-07-17 14:48:08.675] [info] yue
    // scheduleB(func, 5, std::ref(w), std::ref(b), "rui");
    // spdlog::info("{}", w.str()); // [2021-07-17 14:48:08.675] [info] rui

    // scheduleL(func, 5, w, b, "rui"); // use of deleted function ‘Bar::Bar(const Bar&)’
    // scheduleL(func, 5, w, std::ref(b), "rui");
    // spdlog::info("{}", w.str()); // [2021-07-17 14:51:27.136] [info] yue
    // scheduleL(func, 5, std::ref(w), std::ref(b), "rui");
    // spdlog::info("{}", w.str()); // [2021-07-17 14:51:27.137] [info] rui

    // scheduleL2(func, 5, w, b, "rui");
    // spdlog::info("{}", w.str()); // [2021-07-17 15:06:38.444] [info] rui

    // scheduleL3(func, 5, w, b, "rui"); // use of deleted function ‘Bar::Bar(const Bar&)’

#endif

    }

    return 0;
}