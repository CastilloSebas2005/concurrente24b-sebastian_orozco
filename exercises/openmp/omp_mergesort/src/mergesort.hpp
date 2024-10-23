// Copyright 2023 Jeisson Hidalgo jeisson.hidalgo@ucr.ac.cr CC-BY-4
#include <algorithm>
#include <vector>

// TODO(you): parallelize merge sort
template <typename Type>
void mergesort(std::vector<Type> &values, const ptrdiff_t left,
               const ptrdiff_t right) {
  // Count of elements we have to sort
  const ptrdiff_t count = right - left;
  /// Si la cantidad de elementos es mayor o igual a 5000 se realiza el
  /// mergesort si no se realiza un sort esto porque si no se realiza el
  /// mergesort se crean muchas tareas y se pierde tiempo y se vuelve más lenta
  /// que la versión serial
  if (count >= 5000) {
    const size_t mid = (left + right) / 2;
/// Se crea un grupo de tareas para que se realicen en paralelo
#pragma omp taskgroup
    {
// Sort left half of the array
/// Se le asigna una tarea a un hilo para que realice la tarea de mergesort
#pragma omp task default(none) shared(values) firstprivate(left, mid)
        {mergesort(values, left, mid);
  }
// Sort right half of the array
/// Se le asigna una tarea a un hilo para que realice la tarea de mergesort
#pragma omp task default(none) shared(values) firstprivate(mid, right)
  { mergesort(values, mid + 1, right); }
// #pragma omp taskwait
/// Se espera a que ambas tareas terminen para continuar
#pragma omp taskwait
}
// Both halves are sorted, merge them into a temp vector
std::vector<Type> temp;
temp.reserve(count + 1);
std::merge(values.begin() + left, values.begin() + mid + 1,
           values.begin() + mid + 1, values.begin() + right + 1,
           std::back_inserter(temp));
// Copy the sorted temp vector back to the original vector
std::copy(temp.begin(), temp.end(), values.begin() + left);
} else {  // Use any other sorting algorithm
  std::sort(values.begin() + left, values.begin() + right + 1);
  }
}

template <typename Type>
void mergesort(std::vector<Type> &values, const int thread_count) {
/// Se crean los hilos con la cantidad de hilos que se le pase como parametro
#pragma omp parallel default(none) shared(values)                              \
    num_threads(thread_count)
/// Solamente un hilo se encarga de realizar la tarea de mergesort para que no
/// se cree una tarea por cada hilo #pragma omp single a diferencia de #pragma
/// omp critical, asegura que solo un hilo ejecute la tarea
#pragma omp single
  { mergesort(values, 0, values.size() - 1); }
}
