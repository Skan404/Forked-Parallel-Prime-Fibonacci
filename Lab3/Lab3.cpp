#include <iostream>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i <= sqrt(x); i++) {
        if (x % i == 0) return false;
    }
    return true;
}

bool is_fibonnaci(int n) {
    if (n == 0 || n == 1) return true;
    int a = 0, b = 1, c = a + b;
    while (c < n) {
        a = b;
        b = c;
        c = a + b;
    }
    return c == n;
}

int process_range(int start, int end) {
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (is_prime(i) && is_fibonnaci(i)) {
            cout << i << endl;
            count++;
        }
    }
    return count;
}

int main() {
    auto start_time = high_resolution_clock::now();

    const int range = 1000000; // Uwzglêdniamy ograniczenie dla celów demonstracyjnych
    const int segments = 7;
    const int per_child_range = range / segments;
    int total_count = 0;

    for (int i = 0; i < segments; ++i) {
        pid_t pid = fork();
        if (pid == 0) { // Procesy potomne
            int start = i * per_child_range + 1;
            int end = (i + 1) * per_child_range;
            if (i == segments - 1) {
                end = range; // Upewnij siê, ¿e ostatni proces obejmuje ca³y zakres.
            }
            int found = process_range(start, end);
            cout << "Process " << i << " found " << found << " numbers." << endl;
            exit(found);
        }
    }

    // Proces g³ówny czeka na procesy potomne i agreguje wyniki
    int status;
    for (int i = 0; i < segments; ++i) {
        pid_t pid = wait(&status);
        if (WIFEXITED(status)) {
            total_count += WEXITSTATUS(status);
        }
    }

    auto stop_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_time - start_time);

    cout << "Finished processing. Total prime Fibonacci numbers: " << total_count << endl;
    cout << "Execution time: " << duration.count() / 1000000.0 << " seconds." << endl;

    return 0;
}
