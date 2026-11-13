#include <iostream>
#include <vector>
#include <complex>

const double PI = std::acos(-1);

void fft(std::vector<std::complex<double>> &a, bool invert)
{
    int N = (int)a.size();
    for (int i = 1, j = 0; i < N; ++i)
    {
        int bit = N >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            std::swap(a[i], a[j]);
    }
    for (int len = 2; len <= N; len <<= 1)
    {
        double angle = (2 * PI) / len;
        if (invert)
            angle = -angle;
        std::complex<double> wlen = {std::cos(angle), std::sin(angle)};
        for (int i = 0; i < N; i += len)
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
        for (int i = 0; i < N; ++i)
            a[i] /= N;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s;
    std::cin >> s;
    int n = (int)s.length();
    int N = 1;
    while (N <= 2 * (n - 1))
        N <<= 1;
    std::vector<std::complex<double>> A(N);
    for (int i = 0; i < n; ++i)
        A[i] = s[i] - '0';
    fft(A, false);
    for (int i = 0; i < N; ++i)
        A[i] = std::complex<double>(std::norm(A[i]), 0);
    fft(A, true);
    for (int k = 1; k < n; ++k)
    {
        int ans = round(A[k].real());
        std::cout << ans << '\n';
    }
    return 0;
}