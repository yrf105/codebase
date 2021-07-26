add_executable(poll poll.cc)

add_executable(client client.cc)

add_executable(epoll epoll.cc)

add_executable(epoll_read_and_write_test epoll_read_and_write_test.cc)

# add_executable(reactor_test reactor/reactor_test.cc reactor/EventHandler.cc reactor/Handle.cc reactor/InitiationDispatcher.cc reactor/SynchronousEventDemultiplexer.cc)