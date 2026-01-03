//
// Created by Abdullah Faiz ur rahman on 18/12/2025.
//

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <string>
#include <iostream>

struct Benchmark {
   template <typename F, typename... Args>
   static void benchmark(const std::string& name, F&& fn, Args&&... args) {
      const auto start = std::chrono::steady_clock::now();
      std::invoke(fn, args...);
      const auto end = std::chrono::steady_clock::now();
      const auto et = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      std::cout << name << " took: " << et.count() << " milliseconds" << std::endl;
   }

   template <typename F, typename... Args>
   static decltype(auto) benchmark_and_get_result(const std::string& name, F&& fn, Args&&... args) {
      const auto start = std::chrono::steady_clock::now();
      auto res = std::invoke(fn, args...);
      const auto end = std::chrono::steady_clock::now();
      const auto et = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      std::cout << name << " took: " << et.count() << " milliseconds" << std::endl;
      return res;
   }
};

#endif //BENCHMARK_H
