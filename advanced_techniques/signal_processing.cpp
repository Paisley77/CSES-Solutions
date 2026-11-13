#include <iostream>
#include <vector>
#include <complex>

const double PI = std::acos(-1);

void fft(std::vector<std::complex<double>> &a, bool invert)
{
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; ++i)
    {
        int bit = n >> 1;
        for (; bit & j; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            std::swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1)
    {
        double angle = (2 * PI) / len;
        if (invert)
            angle = -angle;
        std::complex<double> wlen(std::cos(angle), std::sin(angle));
        for (int i = 0; i < n; i += len)
        {
            std::complex<double> w(1);
            for (int j = 0; j < len / 2; ++j)
            {
                std::complex<double> u = a[i + j];
                std::complex<double> v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert)
    {
        for (std::complex<double> &x : a)
            x /= n;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    int N = 1;
    while (N < (n + m - 1))
        N <<= 1;
    std::vector<std::complex<double>> A(N), B(N);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> A[i];
    }
    for (int j = 0; j < m; ++j)
    {
        std::cin >> B[m - 1 - j];
    }
    fft(A, false);
    fft(B, false);
    for (int i = 0; i < N; ++i)
        A[i] *= B[i];
    fft(A, true);
    for (int k = 0; k < n + m - 1; ++k)
    {
        long long prod = llround(A[k].real());
        std::cout << prod << '\n';
    }
    return 0;
}