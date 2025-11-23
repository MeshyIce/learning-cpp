#include <exception>
#include <string>
#include <iostream>
#include <initializer_list>

class VectorException : public std::exception {
private:
    std::string errorMessage;

public:
    VectorException(std::string message) {
        this->errorMessage = message;
    }

    const char* what() const override {
        return this->errorMessage.c_str();
    }
};

class VectorIndexOutOfRange : public VectorException {
public:
    VectorIndexOutOfRange(unsigned int index)
        : VectorException("Index " + std::to_string(index) + " is out of range")
    {}
};

class VectorIsEmpty : public VectorException {
public:
    VectorIsEmpty() : VectorException("Vector is empty!") {}
};

template <typename T>
class Vector {
private:
    T* array;
    unsigned int arraySize;
    unsigned int arrayCapacity;

public:
    Vector(std::initializer_list<T> input) {
        // this->arraySize, this->arrayCapacity are set in this->add
        for (T value : input) {
            this->add(value);
        }
    }

    void erase(unsigned int index) {
        if (this->arraySize == 0) {
            throw VectorIsEmpty();
        }
        
        if (index >= this->arraySize) {
            throw VectorIndexOutOfRange(index);
        }

        for (unsigned int i = index; i < this->arraySize - 1; i++) {
            this->array[i] = this->array[i + 1];
        }

        this->arraySize--;
    }

    void reserve(unsigned int newCapacity) {
        if (this->arrayCapacity >= newCapacity) {
            return;
        }

        T* newArray = new T[newCapacity];

        for (unsigned int i = 0; i < this->arraySize; i++) {
            newArray[i] = this->array[i];
        }
        for (unsigned int i = this->arraySize; i < newCapacity; i++) {
            newArray[i] = T();
        }

        delete[] this->array;
        this->array = newArray;
        this->arrayCapacity = newCapacity;
    }

    void clear() {
        delete[] this->array;
        this->array = nullptr;
        this->arraySize = 0;
        this->arrayCapacity = 0;
    }

    void resize(unsigned int newSize, T newValue = T()) {
        if (newSize > this->arrayCapacity) {
            this->reserve(newSize);
        }

        if (newSize > this->arraySize) {
            for (unsigned int i = this->arraySize; i < newSize; i++) {
                this->array[i] = newValue;
            }
        }

        this->arraySize = newSize;
    }

    void add(T value) {
        if (this->arraySize >= this->arrayCapacity) {
            this->reserve(this->arraySize + 1);
        }

        this->array[this->arraySize] = value;
        this->arraySize++;
    }

    void print() {
        for (unsigned int i = 0; i < this->arraySize; i++) {
            std::cout << this->array[i] << ", ";
        }
        std::cout << "\n";
    }

    int size() {
        return this->arraySize;
    }
};

int main()
{
    Vector<int> v{ 1, 2, 3, 4, 5 };
    std::cout << "Initial vector: ";
    v.print();

    v.add(7);
    v.add(8);
    v.add(9);
    std::cout << "After adding 7, 8, 9: ";
    v.print();

    std::cout << "Size: " << v.size() << "\n";

    v.erase(1);
    std::cout << "After erase(1): ";
    v.print();
    std::cout << "Size: " << v.size() << "\n";

    try {
        v.erase(1000);
    }
    catch (const VectorIndexOutOfRange& e) {
        std::cout << "Successfully received exception: " << e.what() << std::endl;
    }

    v.resize(10);
    std::cout << "After resize(10): ";
    v.print();
    std::cout << "Size: " << v.size() << "\n";

    v.clear();
    std::cout << "After clear: ";
    v.print();
    std::cout << "Size: " << v.size() << "\n";

    try {
        v.erase(0);
    }
    catch (const VectorIsEmpty& e) {
        std::cout << "Successfully received exception: " << e.what() << std::endl;
    }
}