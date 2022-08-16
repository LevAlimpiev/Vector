#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<typename T>
class Vector {
private:
	T* mas;
	size_t size_elements, capacity;
	allocator<T> alloc;
	const size_t CAPACITY_MULTIPLIER = 2;

public:
	Vector();
	Vector(size_t n);
	~Vector();

	class iterator {
	private:
		T* cur;
	public:
		iterator(T* p = nullptr) {
			cur = p;
		}
		iterator& operator ++ (int) {
			iterator temp = *this;
			this->cur++;
			return temp;
		}

		iterator& operator ++ () {
			this->cur++;
			return *this;
		}

		iterator& operator -- (int) {
			iterator temp = *this;
			this->cur = this->cur - 1;
			return temp;
		}

		iterator& operator -- () {
			this->cur--;
			return *this;
		}

		iterator& operator = (const iterator& b) {
			this->cur = b.cur;
			return *this;
		}

		bool operator != (const iterator& b) {
			return !(this->cur == b.cur);
		}
		bool operator == (const iterator& b) {
			return this->cur == b.cur;
		}

		T& operator*() { return *cur; }

		iterator operator + (size_t x) {
			return iterator(this->cur + x);
		}

		iterator operator - (size_t x) {
			return iterator(this->cur - x);
		}
	};
	iterator begin() {
		return iterator(mas);
	}
	iterator rbegin() {
		return iterator(mas + size_elements - 1);
	}
	iterator end() {
		return iterator(mas + size_elements);
	}

private:
	void reserve_capacity();
	void reserve_capacity(size_t new_size_elemets);
public:
	void push_back(const T& value);
	void pop_back();
	void prsize_t();
	void clear();
	void resize(const size_t& new_size, const T fill_variable = T());
	void shrink_to_fit();

	size_t& back() { return mas[size_elements - 1]; }
	size_t size() { return size_elements; }
	size_t get_capacity() { return capacity * sizeof(T); }
	bool empty() { return size_elements == 0; }


	T& operator [](const size_t& index);
	Vector<T>& operator =(const Vector<T>& vec);
};


template<typename T>
Vector<T>::Vector(){
	size_elements = 0, capacity = 0;
	mas = nullptr;
}

template<typename T>
Vector<T>::Vector(size_t n){
	size_elements = n;
	capacity = n;
	mas = alloc.allocate(n);
	for (size_t i = 0; i < n; ++i) {
		alloc.construct(mas + i, T());
	}
}

template<typename T>
Vector<T>::~Vector(){
	for (size_t i = 0; i < capacity; i++) {
		alloc.destroy(mas + i);
	}
	alloc.deallocate(mas, capacity);
}

template<typename T>
void Vector<T>::reserve_capacity() {
	if (size_elements < capacity) return;

	size_t old_capacity = capacity;

	if (capacity == 0) capacity = 1;
	else capacity *= CAPACITY_MULTIPLIER;

	T* newmas = alloc.allocate(capacity);

	for (size_t i = 0; i < size_elements; ++i) {
		alloc.construct(newmas + i, mas[i]);
	}
	for (size_t i = 0; i < size_elements; ++i) {
		alloc.destroy(mas + i);
	}
	alloc.deallocate(mas, old_capacity);
	mas = newmas;
}

template<typename T>
void Vector<T>::reserve_capacity(size_t new_size_elements){
	if (new_size_elements < capacity) return;

	size_t old_capacity = capacity;

	while(new_size_elements >= capacity){
		if (capacity == 0) capacity = 1;
		else capacity *= CAPACITY_MULTIPLIER;
	}

	T* newmas = alloc.allocate(capacity);

	for (size_t i = 0; i < size_elements; ++i) {
		alloc.construct(newmas + i, mas[i]);
	}
	for (size_t i = 0; i < size_elements; ++i) {
		alloc.destroy(mas + i);
	}
	alloc.deallocate(mas, old_capacity);
	mas = newmas;
}

template<typename T>
void Vector<T>::push_back(const T &value){
	reserve_capacity();

	alloc.construct(mas + size_elements, value);
	size_elements++;
}

template<typename T>
void Vector<T>::pop_back(){
	alloc.destroy(mas + size_elements - 1);
	size_elements--;
}
template<typename T>
void Vector<T>::prsize_t(){
	for (size_t i = 0; i < size_elements; ++i) cout << mas[i] << " ";
	cout << '\n';
}

template<typename T>
void Vector<T>::clear(){
	while (!empty()) pop_back();
}

template<typename T>
void Vector<T>::resize(const size_t& new_size, const T fill_variable){
	if (new_size == size_elements) return;

	if (new_size < size_elements) {
		while (new_size < size_elements) pop_back();
	}
	else {
		reserve_capacity(new_size);
		for (size_t i = size_elements; i < new_size; ++i) {
			alloc.construct(mas + i, fill_variable);
		}
		size_elements = new_size;
	}
}

template<typename T>
void Vector<T>::shrink_to_fit(){
	T* new_mas = alloc.allocate(size_elements);
	for (size_t i = 0; i < size_elements; ++i) {
		alloc.construct(new_mas + i, mas[i]);
		alloc.destroy(mas + i);
	}
	alloc.deallocate(mas, capacity);
	mas = new_mas;
	capacity = size_elements;

}


template<typename T>
T& Vector<T>::operator[](const size_t &index) {
	return mas[index];
}

template<typename T>
Vector<T>& Vector<T>::operator =(const Vector<T>& vec) {
	for (size_t i = 0; i < size_elements; ++i) {
		alloc.destroy(mas + i);
	}
	alloc.deallocate(mas, capacity);

	mas = alloc.allacate(vec.size_elements);

	size_elements = vec.size_elements;
	capacity = vec.capacity;

	for (size_t i = 0; i < size_elements; i++) {
		alloc.construct(mas + i, vec.mas[i]);
	}

	return *this;
}






void solve() {
	Vector<size_t> a;
	for (size_t i = 0; i < 100; ++i) a.push_back(i);
	cout << a.get_capacity() << "\n";
	a.shrink_to_fit();
	cout << a.get_capacity() << "\n";
}

int main()
{
	solve();
	return 0;
}

