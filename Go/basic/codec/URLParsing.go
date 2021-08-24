package mycodec

import (
	"fmt"
	"net/url"
)

func URLParsing() {

	p := fmt.Println

	s := "scheme://user:pass@host.com:5432/path?k=v&&k1=v1#f"

	u, err := url.Parse(s)
	if err != nil {
		p(err)
	}

	p(u.Scheme)
	p(u.User)
	p(u.User.Password())
	p(u.Host)
	p(u.Hostname())
	p(u.Port())
	p(u.Path)
	p(u.Query())
	p(u.Fragment)
}