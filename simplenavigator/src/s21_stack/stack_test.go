package s21_stack

import "testing"

func TestStack_Valid_NewStack(t *testing.T) {
	s := NewStack()
	if s == nil {
		t.Fatal("NewStack returned nil")
	}
	if !s.IsEmpty() || s.Len() != 0 {
		t.Fatalf("new stack should be empty, got len=%d", s.Len())
	}
}

func TestStack_Valid_Push(t *testing.T) {
	s := NewStack()
	for i, value := range []int{10, 20, 30} {
		s.Push(value)
		if got := s.Len(); got != i+1 {
			t.Fatalf("after Push(%d), Len()=%d, want %d", value, got, i+1)
		}
		if got, ok := s.Top(); !ok || got != value {
			t.Fatalf("Top()=(%d, %v), want (%d, true)", got, ok, value)
		}
	}
}

func TestStack_LIFO_Pop(t *testing.T) {
	s := NewStack()
	for _, value := range []int{1, 2, 3} {
		s.Push(value)
	}

	for _, want := range []int{3, 2, 1} {
		got, ok := s.Pop()
		if !ok || got != want {
			t.Fatalf("Pop()=(%d, %v), want (%d, true)", got, ok, want)
		}
	}
	if !s.IsEmpty() || s.Len() != 0 {
		t.Fatalf("stack should be empty after popping all values, len=%d", s.Len())
	}
}

func TestStack_Empty_Pop(t *testing.T) {
	s := NewStack()
	if got, ok := s.Pop(); ok || got != 0 {
		t.Fatalf("empty Pop()=(%d, %v), want (0, false)", got, ok)
	}
}

func TestStack_Empty_Top(t *testing.T) {
	s := NewStack()
	if got, ok := s.Top(); ok || got != 0 {
		t.Fatalf("empty Top()=(%d, %v), want (0, false)", got, ok)
	}
}

func TestStack_NonDestructive_Top(t *testing.T) {
	s := NewStack()
	s.Push(42)

	if got, ok := s.Top(); !ok || got != 42 {
		t.Fatalf("Top()=(%d, %v), want (42, true)", got, ok)
	}
	if got := s.Len(); got != 1 {
		t.Fatalf("Len() after Top()=%d, want 1", got)
	}
	if got, ok := s.Pop(); !ok || got != 42 {
		t.Fatalf("Pop() after Top()=(%d, %v), want (42, true)", got, ok)
	}
}
