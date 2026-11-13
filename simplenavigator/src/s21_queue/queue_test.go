package s21_queue

import "testing"

func TestQueue_Valid_NewQueue(t *testing.T) {
	q := NewQueue()
	if q == nil {
		t.Fatal("NewQueue returned nil")
	}
	if !q.IsEmpty() || q.Len() != 0 {
		t.Fatalf("new queue should be empty, got len=%d", q.Len())
	}
}

func TestQueue_FIFO_Pop(t *testing.T) {
	q := NewQueue()
	for i, value := range []int{10, 20, 30} {
		q.Push(value)
		if got := q.Len(); got != i+1 {
			t.Fatalf("after Push(%d), Len()=%d, want %d", value, got, i+1)
		}
	}

	for _, want := range []int{10, 20, 30} {
		got, ok := q.Pop()
		if !ok || got != want {
			t.Fatalf("Pop()=(%d, %v), want (%d, true)", got, ok, want)
		}
	}
	if !q.IsEmpty() || q.Len() != 0 {
		t.Fatalf("queue should be empty after popping all values, len=%d", q.Len())
	}
}

func TestQueue_Valid_Front(t *testing.T) {
	q := NewQueue()
	q.Push(1)
	q.Push(2)
	q.Push(3)

	if got, ok := q.Front(); !ok || got != 1 {
		t.Fatalf("Front()=(%d, %v), want (1, true)", got, ok)
	}
	q.Pop()
	if got, ok := q.Front(); !ok || got != 2 {
		t.Fatalf("Front() after Pop=(%d, %v), want (2, true)", got, ok)
	}
}

func TestQueue_Valid_Back(t *testing.T) {
	q := NewQueue()
	q.Push(1)
	q.Push(2)
	q.Push(3)

	if got, ok := q.Back(); !ok || got != 3 {
		t.Fatalf("Back()=(%d, %v), want (3, true)", got, ok)
	}

	q.Pop()
	if got, ok := q.Back(); !ok || got != 3 {
		t.Fatalf("Back() after Pop=(%d, %v), want (3, true)", got, ok)
	}
}

func TestQueue_Empty_Pop(t *testing.T) {
	q := NewQueue()
	if got, ok := q.Pop(); ok || got != 0 {
		t.Fatalf("empty Pop()=(%d, %v), want (0, false)", got, ok)
	}
}

func TestQueue_Empty_Front(t *testing.T) {
	q := NewQueue()
	if got, ok := q.Front(); ok || got != 0 {
		t.Fatalf("empty Front()=(%d, %v), want (0, false)", got, ok)
	}
}

func TestQueue_Empty_Back(t *testing.T) {
	q := NewQueue()
	if got, ok := q.Back(); ok || got != 0 {
		t.Fatalf("empty Back()=(%d, %v), want (0, false)", got, ok)
	}
}

func TestQueue_Reused_Pop(t *testing.T) {
	q := NewQueue()
	q.Push(1)
	q.Pop()
	q.Push(2)

	if got, ok := q.Pop(); !ok || got != 2 {
		t.Fatalf("Pop() after reuse=(%d, %v), want (2, true)", got, ok)
	}
}

func TestQueue_Compacted_Pop(t *testing.T) {
	q := NewQueue()
	for value := 0; value < 100; value++ {
		q.Push(value)
	}

	for want := 0; want < 60; want++ {
		got, ok := q.Pop()
		if !ok || got != want {
			t.Fatalf("Pop()=(%d, %v), want (%d, true)", got, ok, want)
		}
	}

	if got := len(q.data); got > 80 {
		t.Fatalf("queue retained dequeued prefix: len(data)=%d, want at most 80", got)
	}
	for want := 60; want < 100; want++ {
		got, ok := q.Pop()
		if !ok || got != want {
			t.Fatalf("Pop() after compaction=(%d, %v), want (%d, true)", got, ok, want)
		}
	}
}
