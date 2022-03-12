#include "document_analyzer.h"

int main(void)
{
    load_document("test.txt");
    analyze_document();
    
    dispose();
    
    return 0;
}
