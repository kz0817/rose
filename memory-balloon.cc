#include <iostream>
#include <unistd.h>
#include "ArgParser.h"
using namespace std;
using namespace Leaf;

static constexpr size_t blockSize = sizeof(uint64_t);

static auto leakedAllocAndFill(const size_t sizeMiB)
{
    const size_t numBlocks = sizeMiB * 1024 * 1024 / blockSize;
    auto p = new uint64_t[numBlocks];
    for (size_t i = 0; i < numBlocks; i++)
        p[i] = i;
    return p;
}

static void alloc(const int start, const int delta, const int end)
{
    leakedAllocAndFill(start);
    int currSize = start;
    if (delta < 0)
        return;

    while (end < 0 || currSize < end) {
        leakedAllocAndFill(delta);
        currSize += delta;
        cout << "Allocated: " << currSize << " MiB" << endl;
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    int start = 0;
    int delta = -1;
    int end = -1;

    SimpleArgParser parser;
    parser.add("-s", [&]{ start = atoi(parser.getNext()); },
               "START", "Initial allocation (MiB)");
    parser.add("-d", [&]{ delta = atoi(parser.getNext()); },
               "DELTA", "Allocation size per second (MiB). Negative value means no increment");
    parser.add("-e", [&]{ end   = atoi(parser.getNext()); },
               "END", "Last size (MiB). Negative value means endless");
    parser.parse(argc, argv);
    if (parser.hasError()) {
        cerr << parser.generateUsage() << endl;
        return EXIT_FAILURE;
    }

    cout << "Start: " << start << ", Delta: " << delta
         << ", End: " << end << endl;

    cout << "Block size: " << blockSize << endl;
    alloc(start, delta, end);

    return EXIT_SUCCESS;
}
