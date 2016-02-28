#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int T;
typedef bool (*cmp_func_t)(const T &a, const T &b); 

void Swap(T &a, T &b)
{
  T t;
  t = a;
  a = b;
  b = t;
}

// operation <   order
bool cmp_less(const T &leftData, const T &rightData)
{
  return (rightData - leftData) >= 0 ? 1 : 0;
}

// operation >  reversed order
bool cmp_bigger(const T &leftData, const T &rightData)
{
  return (rightData - leftData) >= 0 ? 0 : 1;
}

void printArr(T const *arr, int n)
{
  for(int i = 0; i < n; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");
}


/**********************************************
***************** Select  Sort  ***************
**********************************************/
void SelectSort(T *arr, int n, cmp_func_t cmp_fn)
{
  int i, j, minmaxIndex;
  for(i = 0; i < n-1; i++)
  {
    minmaxIndex = i;
    for(j = i+1; j < n; j++)
    {
      if( cmp_fn(arr[j], arr[minmaxIndex]) )
      {
        minmaxIndex = j;
      }
    }
    // find the min/max from unsorted list,
    // swap it with the first element of unsorted list
    if(minmaxIndex != i)
      Swap(arr[minmaxIndex], arr[i]);
  }
}


/**********************************************
***************** Insert  Sort  ***************
**********************************************/
void InsertSort(T *arr, int n, cmp_func_t cmp_fn)
{
  int i, j;
  T temp;
  for(i = 1; i < n; i++)
  {
    temp = arr[i];
    // right shifting element while searching insert point
    for(j = i-1; j >= 0; j--)
    {
      if( cmp_fn(temp, arr[j]) )
        arr[j+1] = arr[j];
      else
        break;
    }
    arr[j+1] = temp;
  }
}


/**********************************************
***************** Bubble  Sort  ***************
**********************************************/
void BubbleSort(T *arr, int n, cmp_func_t cmp_fn)
{
  int i;
  bool isSwaped;
  // n-1 times
  while(--n > 0)
  {
    isSwaped = false;
    for(i = 0; i < n; i++)
    {
      if( cmp_fn(arr[i+1], arr[i]) )
      {
        Swap(arr[i+1], arr[i]);
        isSwaped = true;
      }
    }
    if(!isSwaped)
      break;
  }
}

void Bubble2Sort(T *arr, int n, cmp_func_t cmp_fn)
{
  int low = 0;
  int high = n - 1;
  int i, t;
  while(low < high)
  {
    // 正向部分
    for(i = low; i < high; i++)
    {
      if( cmp_fn(arr[i+1], arr[i]) )
      {
        Swap(arr[i], arr[i+1]);
        t = i; // 待排序尾端
      }
    }
    high = t;

    // 反向部分
    for(i = high; i > low; i--)
    {
      if( cmp_fn(arr[i], arr[i-1]) )
      {
        Swap(arr[i], arr[i-1]);
        t = i; // 待排序始端
      }
    }
    low = t;
  }

}


/**********************************************
***************** Merge  Sort  ***************
**********************************************/
void Merge(T *arr, T *temp, int first, int mid, int last, cmp_func_t cmp_fn)
{
  int i = first, j = mid + 1;
  int k = 0;
  while(i <= mid && j <= last)
  {
    if( cmp_fn(arr[i], arr[j]) )  
      temp[k++] = arr[i++];   // 取arr[i]和arr[j]中较小/较大者放入temp[k]
    else
      temp[k++] = arr[j++];
  }

  while(i <= mid)
    temp[k++] = arr[i++]; // 若第一个子序列没处理完，则进行收尾处理
  while(j <= last)
    temp[k++] = arr[j++]; // 若第二个子序列没处理完，则进行收尾处理

  for(i = 0; i < k; i++)
    arr[first+i] = temp[i];
}

void MergeSort(T *arr, T *temp, int first, int last, cmp_func_t cmp_fn)
{
  if(first < last)
  {
    int mid = (first + last) / 2;
    MergeSort(arr, temp, first, mid, cmp_fn);
    MergeSort(arr, temp, mid+1, last, cmp_fn);
    Merge(arr, temp, first, mid, last, cmp_fn);

  }
}


// 非递归
void MergeSort2(T *arr, T *temp, int n, cmp_func_t cmp_fn)
{
  int first, mid, last;
  int i, step;
  // step = 1, 2, 4, 8, ... log(n)
  for(step = 1; step < n; step *= 2)
  {
    for(i = 0; i < n; i += 2*step)
    {
      first = i;
      last = i + 2*step - 1;
      mid = (first + last) / 2;
      if (mid >= n)
        break;
      if(last >= n)
        last = n - 1;

      Merge(arr, temp, first, mid, last, cmp_fn);
    }
  }
}


/**********************************************
***************** Quick  Sort  ***************
**********************************************/
int Partition(T *arr, int start, int end, cmp_func_t cmp_fn)
{
  // 取中间值为轴值
  int index = (start + end) / 2;
  Swap(arr[end], arr[index]);
  int pivot = arr[end]; // 轴值

  // 分割
  int leftId = start;
  for(index = start; index < end; ++index)
  {
    if( cmp_fn(arr[index], pivot) )
    {
      if(leftId != index)
        Swap(arr[index], arr[leftId]);
      ++leftId;
    }
  }

  Swap(arr[leftId], arr[end]);
  return leftId;
}

void QuickSort(T *arr, int start, int end, cmp_func_t cmp_fn)
{
  if(start == end)
    return;

  int index = Partition(arr, start, end, cmp_fn);
  if(index > start)
    QuickSort(arr, start, index-1, cmp_fn);
  if(index < end)
    QuickSort(arr, index+1, end, cmp_fn);

}


/**********************************************
***************** Shell  Sort  ***************
**********************************************/
void ShellSort(T *arr, int n, cmp_func_t cmp_fn)
{
  int i, j, gap;
  T temp;
  for(gap = n/2; gap >= 1; gap /= 2)
  {
    for(i = gap; i < n; i++)
    {
      temp = arr[i];
      for(j = i-gap; j >= 0; j -= gap)
      {
        if( cmp_fn(temp, arr[j]) )
          arr[j+gap] = arr[j];
        else
          break;
      }
      arr[j+gap] = temp;
    }
  }
}


/**********************************************
***************** Heap    Sort  ***************
**********************************************/
void HeapSort(T *arr, int n, cmp_func_t cmp_fn)
{

}

int main()
{
  //int a[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  int a[5] = {25, 13, 51, 23, 7};
  int n = sizeof(a) / sizeof(a[0]);
  T *temp = (T*)malloc(sizeof(T)*n);

  printArr(a, n); 

  //InsertSort(a, n, cmp_bigger);
  //Bubble2Sort(a, n, cmp_bigger);
  //MergeSort(a, temp, 0, n-1, cmp_less);
  //MergeSort2(a, temp, n, cmp_less);
  //QuickSort(a, 0, n-1, cmp_bigger);
  ShellSort(a, n, cmp_less);
  printArr(a, n);
  return 0;
}
