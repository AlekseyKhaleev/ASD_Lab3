#pragma once

#include <fstream>
#include <iostream>


template<typename T>
class List
{
public:
   //Конструкторы
   List();

   List(List<T> const &); //copy
   List<T> &operator=(const List<T> &);

   List(List<T> &&) noexcept; //move
   List<T> &operator=(List<T> &&) noexcept;

   explicit List(const char *);

   //Деструктор
   ~List();

   //Методы
   void PushBack(T);

   T PopFront();

   void RemoveAt(int);

   void ReadFromFile(char *);

   void WriteToFile(const char *path= nullptr);

   void DeleteAllSeq(List<T>);

   int GetSize() const;

   [[nodiscard]] T Ind(int index) const;

   void Clear();

private:
   class Node
   {
   public:
      Node *pNext;
      T data;

      explicit Node(T data, Node *pNext = nullptr)
      {
         this->data = data;
         this->pNext = pNext;
      }
   };

   int size;
   Node *head;

   int FindFirst(List<T> image);

   List<int> PrefixFunction(List<T> image);
};

// Конструкторы

//default
template<typename T>
List<T>::List()
{
   size = 0;
   head = nullptr;
}

//copy
template<typename T>
List<T>::List(const List<T> &other)
{
   size = 0;
   head = nullptr;
   for (int i = 0; i < other.size; i++)
   {
      PushBack(other.Ind(i));
   }
}

template<typename T>
List<T> &List<T>::operator=(const List<T> &other)
{
   if (&other != this)
   {
      Clear();
      for (int i = 0; i < other.size; i++)
      {
         PushBack(other.Ind(i));
      }
   }
   return *this;
}


//move
template<typename T>
List<T>::List(List<T> &&other) noexcept
{
   size = other.size;
   head = other.head;
}

template<typename T>
List<T> &List<T>::operator=(List<T> &&other) noexcept
{
   if (&other != this)
   {
      Clear();
      size = other.size;
      head = other.head;
   }
   return *this;
}

template<typename T>
List<T>::List(const char *str)
{
   size = 0;
   while (*(str + size) != '\0')
   {
      PushBack(*(str + size));
   }
}


// Деструктор
template<typename T>
List<T>::~List()
{
   Clear();
}

template<typename T>
void List<T>::PushBack(T data)
{
   if (head == nullptr)
   {
      head = new Node(data);
   } else
   {
      Node *current = this->head;
      while (current->pNext != nullptr)
      {
         current = current->pNext;
      }
      current->pNext = new Node(data);
   }
   size++;
}

template<typename T>
int List<T>::GetSize() const {
   return this->size;
}

template<typename T>
T List<T>::PopFront()
{
   T value = head->data;
   Node *temp = head;
   head = head->pNext;
   delete temp;
   size--;
    return value;
}

template<typename T>
void List<T>::Clear()
{
   while (size)
   {
      this->PopFront();
   }
}

template<typename T>
void List<T>::RemoveAt(int index)
{
   if (index > size - 1)
   {
      throw std::exception();
   }

   if (index == 0)
   {
      PopFront();
   } else
   {
      Node *previous = this->head;
      for (int i = 0; i < index - 1; i++)
      {
         previous = previous->pNext;
      }
      Node *temp = previous->pNext;
      previous->pNext = temp->pNext;
      delete[] temp;
   }
   size--;
}

template<typename T>
void List<T>::ReadFromFile(char *path)
{
   Clear();
   std::fstream file(path, std::ios::in);
   if (file.is_open())
   {
      T buf;
      file.read(&buf, sizeof(T));
      while (!file.eof())
      {
         this->PushBack(buf);
         file.read(&buf, sizeof(T));

      }
   }
   file.close();
   if (file.is_open())
   {
      throw std::exception();
   }
}

template<typename T>
void List<T>::WriteToFile(const char *path)
{
   if (path == nullptr)
   {
      path = "../output_file";
   }
   std::ofstream file(path, std::ios::out | std::ios::trunc);
   if (file.is_open())
   {
      Node *tmp = head;
      for (int i = 0; i < size; i++)
      {
         file.write(&tmp->data, sizeof(T));
         tmp = tmp->pNext;
      }
   }
   file.close();
   if (file.is_open())
   {
      throw std::exception();
   }
   else
   {
      std::cout<<"\n   The modified text was saved in: "<< path<< "\n\n";
   }
}

template<typename T>
void List<T>::DeleteAllSeq(List<T> image)
{
   int index = FindFirst(image);
   while (index >= 0)
   {
      for (int i = 0; i < image.GetSize(); i++)
      {
         RemoveAt(index);
      }
      index = FindFirst(image);
   }
}

template<typename T>
List<int> List<T>::PrefixFunction(List<T> image)
{
   List<int> pi{};// В i-м элементе (его индекс i-1) количество совпавших символов в начале и конце для подстроки длины i.
   // p[0]=0 всегда, p[1]=1, если начинается с двух одинаковых
   pi.PushBack(0);
   for (int i = 1; i < image.GetSize(); ++i)
   {
      // ищем, какой префикс-суффикс можно расширить
      int j = pi.Ind(i - 1); // длина предыдущего префикса-суффикса, возможно нулевая
      while ((j > 0) && (image.Ind(i) != image.Ind(j)))// этот нельзя расширить,
      {
         j = pi.Ind(j - 1);   // берем длину меньшего префикса-суффикса
      }
      if (image.Ind(i) == image.Ind(j))
      {
         ++j;  // расширяем найденный (возможно пустой) префикс-суффикс
      }
      pi.PushBack(j);
   }
   return pi;
}

template<typename T>
int List<T>::FindFirst(List<T> image)
{
   List<int> pi = PrefixFunction(image);
   int txtLen = this->GetSize(), imgLen = image.GetSize();
   for (int k = 0, l = 0; k < txtLen;)
   {
      if (this->Ind(k) != image.Ind(l))
      {
         l == 0 ? k++ : l = pi.Ind(l - 1);
      } else
      {
         if (l == imgLen - 1)
         {
            return (k - imgLen + 1);
         }
         k++;
         l++;
      }
   }
   return -1;
}

template<typename T>
T List<T>::Ind(int index) const
{
   int counter{0};
   Node *current = this->head;
   while (current != nullptr)
   {
      if (counter == index)
      {
         return current->data;
      }
      current = current->pNext;
      counter++;
   }
   throw std::exception();
}










