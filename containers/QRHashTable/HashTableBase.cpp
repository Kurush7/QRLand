//
// Created by kurush on 29.04.2020.
//

#include "HashTableBase.h"

using namespace std;

HashTableBase::HashTableBase(): curSize(0), capacity(make_shared<size_t>(0)) {}
HashTableBase::HashTableBase(size_t capacity)
: curSize(0), capacity(make_shared<size_t>(capacity)) {}
HashTableBase::HashTableBase(size_t curSize, size_t capacity)
: curSize(curSize), capacity(make_shared<size_t>(capacity)) {}

HashTableBase::~HashTableBase() {};

HashTableBase::operator bool() const {return curSize != 0;}

size_t HashTableBase::size() const {return curSize;}