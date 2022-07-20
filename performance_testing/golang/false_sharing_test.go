package main

import "testing"

func BenchmarkPadIncrease(b *testing.B) {
	p := &pad{}
	b.RunParallel(func(pb *testing.PB) {
		for pb.Next() {
			p.Increase()
		}
	})
}

func BenchmarkNoPadIncrease(b *testing.B) {
	p := &noPad{}
	b.RunParallel(func(pb *testing.PB) {
		for pb.Next() {
			p.Increase()
		}
	})
}

