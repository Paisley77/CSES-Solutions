#include <iostream>

char query(int i)
{
    char color;
    std::cout << "? " << i << '\n';
    std::cout.flush();
    std::cin >> color;
    return color;
}

int main()
{
    int n;
    std::cin >> n;
    int lo = 1, hi = n + 1;
    char colorLo = query(1);
    char colorHi = colorLo;

    while (hi - lo > 1)
    {
        int mid = (hi + lo) >> 1;
        char colorMid = query(mid);
        int sLeft = (colorMid != colorLo);
        bool leftOK = (sLeft && (mid - lo) % 2 == 0) || (!sLeft && (mid - lo) % 2 != 0);
        if (leftOK)
        {
            hi = mid;
            colorHi = colorMid;
        }
        else
        {
            lo = mid;
            colorLo = colorMid;
        }
    }

    std::cout << "! " << lo << std::endl;
    return 0;
}