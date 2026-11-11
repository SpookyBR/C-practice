#include <stdbool.h>

bool jogoDivisor(int n) {
    if (n <= 1) {
        return false;
    }

    bool dp[1005] = {false};

    bool dp[1] = false;

    for (int i = 2; i <= n; i++) {
        for (int x = 1; x < i; x++) {
            if (i % x = 0 && !dp[i - x]) {
                dp[i] = true;
                break;
            }
        }
    }

    return dp[n];
}