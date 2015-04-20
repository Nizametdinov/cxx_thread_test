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

void test_no_threads() {
  steady_clock::time_point begin, end;

  begin = steady_clock::now();
  double result = trapezoidal(&ln1plusX, 0, 1);
  end = steady_clock::now();
  printf("No additional threads time: %lldms\n", duration_cast<milliseconds>(end - begin).count());
  printf("Result: %.8f\n", result);
}

void test_threads(int num_threads) {
  steady_clock::time_point begin, end;

  begin = steady_clock::now();
  double result = integrate_with_threads(num_threads);
  end = steady_clock::now();

  printf("%d threads result: %.8f\n", num_threads, result);
  printf("%d threads time:   %lldms\n", num_threads, duration_cast<milliseconds>(end - begin).count());
}

int main() {
  test_no_threads();

  for(int i = 1; i <= 8; ++i) {
    test_threads(i);
  }

  return 0;
}
