#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<typename T>
class Vector {
private:
	T* mas;
	int size_elements, capacity;
	allocator<T> alloc;
	const int CAPACITY_MULTIPLIER = 2;

public:
	Vector();
	Vector(int n);
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

		iterator operator + (int x) {
			return iterator(this->cur + x);
		}

		iterator operator - (int x) {
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
	void reserve_capacity(int new_size_elemets);
public:
	void push_back(const T& value);
	void pop_back();
	void print();
	void clear();
	void resize(const int& new_size, const T fill_variable = T());
	void shrink_to_fit();

	int& back() { return mas[size_elements - 1]; }
	int size() { return size_elements; }
	size_t get_capacity() { return capacity * sizeof(T); }
	bool empty() { return size_elements == 0; }


	T& operator [](const int& index);
	Vector<T>& operator =(const Vector<T>& vec);
};


template<typename T>
Vector<T>::Vector(){
	size_elements = 0, capacity = 0;
	mas = nullptr;
}

template<typename T>
Vector<T>::Vector(int n){
	size_elements = n;
	capacity = n;
	mas = alloc.allocate(n);
	for (int i = 0; i < n; ++i) {
		alloc.construct(mas + i, T());
	}
}

template<typename T>
Vector<T>::~Vector(){
	for (int i = 0; i < capacity; i++) {
		alloc.destroy(mas + i);
	}
	alloc.deallocate(mas, capacity);
}

template<typename T>
void Vector<T>::reserve_capacity() {
	if (size_elements < capacity) return;

	int old_capacity = capacity;

	if (capacity == 0) capacity = 1;
	else capacity *= CAPACITY_MULTIPLIER;

	T* newmas = alloc.allocate(capacity);

	for (int i = 0; i < size_elements; ++i) {
		alloc.construct(newmas + i, mas[i]);
	}
	for (int i = 0; i < size_elements; ++i) {
		alloc.destroy(mas + i);
	}
	alloc.deallocate(mas, old_capacity);
	mas = newmas;
}

template<typename T>
void Vector<T>::reserve_capacity(int new_size_elements){
	if (new_size_elements < capacity) return;

	int old_capacity = capacity;

	while(new_size_elements >= capacity){
		if (capacity == 0) capacity = 1;
		else capacity *= CAPACITY_MULTIPLIER;
	}

	T* newmas = alloc.allocate(capacity);

	for (int i = 0; i < size_elements; ++i) {
		alloc.construct(newmas + i, mas[i]);
	}
	for (int i = 0; i < size_elements; ++i) {
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
void Vector<T>::print(){
	for (int i = 0; i < size_elements; ++i) cout << mas[i] << " ";
	cout << '\n';
}

template<typename T>
void Vector<T>::clear(){
	while (!empty()) pop_back();
}

template<typename T>
void Vector<T>::resize(const int& new_size, const T fill_variable){
	if (new_size == size_elements) return;

	if (new_size < size_elements) {
		while (new_size < size_elements) pop_back();
	}
	else {
		reserve_capacity(new_size);
		for (int i = size_elements; i < new_size; ++i) {
			alloc.construct(mas + i, fill_variable);
		}
		size_elements = new_size;
	}
}

template<typename T>
void Vector<T>::shrink_to_fit(){
	for (int i = capacity - 1; i > size_elements; i--, capacity--) {
		alloc.destroy(mas + i);
		//alloc.deallocate(mas + i, capacity);
	}

}


template<typename T>
T& Vector<T>::operator[](const int &index) {
	return mas[index];
}

template<typename T>
Vector<T>& Vector<T>::operator =(const Vector<T>& vec) {
	for (int i = 0; i < size_elements; ++i) {
		alloc.destroy(mas + i);
	}
	alloc.deallocate(mas, capacity);

	mas = alloc.allacate(vec.size_elements);

	size_elements = vec.size_elements;
	capacity = vec.capacity;

	for (int i = 0; i < size_elements; i++) {
		alloc.construct(mas + i, vec.mas[i]);
	}

	return *this;
}






void solve() {
	Vector<int> a;
	for (int i = 0; i < 5; ++i) a.push_back(i);
	cout << a.get_capacity() << "\n";
	a.shrink_to_fit();
	cout << a.get_capacity() << "\n";
}

int main()
{
	solve();
	return 0;
}

