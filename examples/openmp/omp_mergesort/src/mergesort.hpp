// Copyright 2023 Jeisson Hidalgo jeisson.hidalgo@ucr.ac.cr CC-BY-4
#include <omp.h>
#include <algorithm>
#include <vector>

/// @brief Sorts a vector of values using mergesort algorithm
/// @tparam Type
/// @param values values to sort 
/// @param left Left index of the range to sort
/// @param right Right index of the range to sort
template <typename Type>
void mergesort(std::vector<Type>& values, const ptrdiff_t left,
    const ptrdiff_t right) {
  // Count of elements we have to sort
  const ptrdiff_t count = right - left;
  // If there are enough data to justify mergesort, do it
  if (count >= 1024) {
    const size_t mid = (left + right) / 2;
    /// Esto lo que hace es crear un grupo de tareas, y se ejecutan en paralelo
    /// sin embargo, se espera a que todas las tareas del grupo terminen para
    /// continuar con la ejecución del código que sigue después del grupo de tareas en ese momento
    /// Es una barrera implícita
    #pragma omp taskgroup
    {
<<<<<<< HEAD
      // Sort left half of the array
      /// Se crean dos tareas, una para el subarreglo izquierdo y otra para el subarreglo derecho
      /// Se espera a que ambas tareas terminen para continuar
      /// Note que se usa el untied para que el hilo que crea la tarea no se quede esperando a que la tarea termine
      /// y pueda seguir, sin embargo esto solo se aplica si el número de elementos a ordenar es mayor o igual a 5000
      /// y firstprivate para que cada tarea tenga su propia copia de left y mid y no se compartan entre las tareas
      /// pero no son private para que no se creen copias de left y mid por cada hilo
      #pragma omp task default(none) untied if(count >= 5000) \
          shared(values) firstprivate(left, mid)
      mergesort(values, left, mid);
      // Sort right half of the array
      /// Esta es la tarea que se ejecuta en paralelo con la tarea anterior
      /// Se espera a que ambas tareas terminen para continuar
      /// Note que se usa el untied para que el hilo que crea la tarea no se quede esperando a que la tarea termine al igual que la tarea anterior
      #pragma omp task default(none) untied if(count >= 5000) \
          shared(values) firstprivate(mid, right)
      mergesort(values, mid + 1, right);
      //#pragma omp taskwait
      /// Se puede usar taskyield para que el hilo que crea la tarea no se quede esperando a que la tarea termine
      /// y pueda seguir.
      #pragma omp taskyield
    }
    // Both halves are sorted, merge them into a temp vector
    /// Merge sort normal
    std::vector<Type> temp;
    temp.reserve(count + 1);
    std::merge(values.begin() + left, values.begin() + mid + 1,
        values.begin() + mid + 1, values.begin() + right + 1,
        std::back_inserter(temp));
    // Copy the sorted temp vector back to the original vector
    std::copy(temp.begin(), temp.end(), values.begin() + left);
  } else {  // Use any other sorting algorithm
    std::sort(values.begin() + left, values.begin() + right + 1);
=======
// Sort left half of the array
/// Se le asigna una tarea a un hilo para que realice la tarea de mergesort
#pragma omp task default(none) shared(values) firstprivate(left, mid)
        {mergesort(values, left, mid);
  }
// Sort right half of the array
/// Se le asigna una tarea a un hilo para que realice la tarea de mergesort
#pragma omp task default(none) shared(values) firstprivate(mid, right)
  { mergesort(values, mid + 1, right); }
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
>>>>>>> b6fab6dc278f1a1efbe5e494c75ffa7ca74ee457
  }
  /// Termina la barrera implícita
}

/// @brief Sorts a vector of values using mergesort algorithm
/// En esta versión se crean los hilos en la función mergesort
/// y se espera a que terminen de ejecutar el mergesort de los dos
/// subarreglos para luego hacer el merge de los dos subarreglos
/// @tparam Type 
/// @param values Values to sort
/// @param thread_count Number of threads to use
template <typename Type>
<<<<<<< HEAD
void mergesort(std::vector<Type>& values,
    const int thread_count = omp_get_max_threads()) {
  /// Se crean los hilos en la función mergesort
  #pragma omp parallel num_threads(thread_count) default(none) shared(values)
  /// Solo un hilo ejecuta el mergesort de todo el arreglo, puede ser cualquier hilo
  #pragma omp single
  mergesort(values, 0, values.size() - 1);
=======
void mergesort(std::vector<Type> &values, const int thread_count) {
/// Se crean los hilos con la cantidad de hilos que se le pase como parametro
#pragma omp parallel default(none) shared(values)                              \
    num_threads(thread_count)
/// Solamente un hilo se encarga de realizar la tarea de mergesort para que no
/// se cree una tarea por cada hilo #pragma omp single a diferencia de #pragma
/// omp single, asegura que solo un hilo ejecute la tarea
#pragma omp single
  { mergesort(values, 0, values.size() - 1); }
>>>>>>> b6fab6dc278f1a1efbe5e494c75ffa7ca74ee457
}
