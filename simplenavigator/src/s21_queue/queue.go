package s21_queue

type Queue struct {
	data []int
	head int
}

func NewQueue() *Queue {
	return &Queue{
		data: []int{},
		head: 0,
	}
}

func (q *Queue) Push(value int) {
	q.data = append(q.data, value)
}

func (q *Queue) Pop() (int, bool) {
	if q.IsEmpty() {
		return 0, false
	}
	value := q.data[q.head]
	q.head++
	if q.head == len(q.data) {
		q.data = nil
		q.head = 0
	} else if q.head >= len(q.data)-q.head {
		q.compact()
	}
	return value, true
}

func (q *Queue) Front() (int, bool) {
	if q.IsEmpty() {
		return 0, false
	}
	return q.data[q.head], true
}

func (q *Queue) Back() (int, bool) {
	if q.IsEmpty() {
		return 0, false
	}
	return q.data[len(q.data)-1], true
}

func (q *Queue) Len() int {
	return len(q.data) - q.head
}

func (q *Queue) IsEmpty() bool {
	return q.head >= len(q.data)
}

func (q *Queue) compact() {
	remaining := len(q.data) - q.head
	data := make([]int, remaining)
	copy(data, q.data[q.head:])
	q.data = data
	q.head = 0
}
