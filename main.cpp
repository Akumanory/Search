#include <string>
#include "search.cpp"

int main()
{
    Dictionary<int, int> a;
    a.put(1, 10);
    a.put(3, 20);
    a.put(4, 30);

    a.show();

    a.iterator();

    a.test_min_max();

    std::cout << "Key: " << a[23] << std::endl;

    for (auto it = a.iterator(); it.hasNext(); it.next())
    {
        std::cout << "Key: " << it.key() << std::endl;
    }
}