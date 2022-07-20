package main

import (
	"fmt"
	"sync/atomic"
	"unsafe"
)

type noPad struct {
	a int64
	b int64
	c int64
}

func (p *noPad) Increase() {
	atomic.AddInt64(&p.a, 1)
	atomic.AddInt64(&p.b, 1)
	atomic.AddInt64(&p.c, 1)
}

type pad struct {
	a  int64
	_a [7]int64
	b  int64
	_b [7]int64
	c  int64
	_c [7]int64
}

func (p *pad) Increase() {
	atomic.AddInt64(&p.a, 1)
	atomic.AddInt64(&p.b, 1)
	atomic.AddInt64(&p.c, 1)
}

func main() {
	p := pad{}
	fmt.Println(unsafe.Sizeof(p))
}

