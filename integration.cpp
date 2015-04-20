#include <cmath>
#include <cstdio>
#include <thread>
#include <vector>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef double (*PFLOAT)(double);

double ln1plusX(double x) {
  return log(1. + x);
}

double trapezoidal(PFLOAT func, double a, double b, double step = 1e-8) {
  double result = 0.;
  for (; a < b; a += step) {
    result += (func(a) + func(a+step)) * step / 2;
  }
  return result;
}

void thread_func(double *result, double begin, double end) {
  *result = trapezoidal(&ln1plusX, begin, end);
}

double integrate_with_threads(int num_threads) {
  vector<thread> threads;
  double *results = new double[num_threads];

  for (int i = 0; i < num_threads; ++i) {
    threads.push_back(thread(thread_func, results + i, i * 1./num_threads, (i + 1) * 1./num_threads));
  }

  for (int i = 0; i < num_threads; ++i) {
    threads[i].join();
  }

  double result = 0.;
  for (int i = 0; i < num_threads; ++i)
    result += results[i];

  delete[] results;
  return result;
}

int main() {
  system_clock::time_point begin, end;
  double result;

  begin = system_clock::now();
  result = integrate_with_threads(1);
  end = system_clock::now();
  printf("1 thread time %lldms\n", duration_cast<milliseconds>(end - begin).count());
  printf("integral = %.8f\n", result);

  begin = system_clock::now();
  result = integrate_with_threads(2);
  end = system_clock::now();
  printf("2 threads time %lldms\n", duration_cast<milliseconds>(end - begin).count());
  printf("integral = %.8f\n", result);

  begin = system_clock::now();
  result = integrate_with_threads(3);
  end = system_clock::now();
  printf("3 threads time %lldms\n", duration_cast<milliseconds>(end - begin).count());
  printf("integral = %.8f\n", result);

  begin = system_clock::now();
  result = integrate_with_threads(4);
  end = system_clock::now();
  printf("4 threads time %lldms\n", duration_cast<milliseconds>(end - begin).count());
  printf("integral = %.8f\n", result);

  begin = system_clock::now();
  result = trapezoidal(&ln1plusX, 0, 1);
  end = system_clock::now();
  printf("No threads time %lldms\n", duration_cast<milliseconds>(end - begin).count());
  printf("integral = %.8f\n", result);
  return 0;
}
