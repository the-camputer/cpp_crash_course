#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <bitset>

/*
    Streams: These model a stream of data flowing between objects, e.g. a file or network connection or the 
    console.

    Three type templates provided are:
    basic_istream  (<istream>)  : represents input device
    basic_ostream  (<ostream>)  : represents output device
    basic_iostream (<iostream>) : represents device that can input and output

    Important implementations are:
    cout: outputs to console
    cin: takes input in (e.g. from keyboard)
    cerr: outputs error (unbuffered)
    clog: outputs error (buffered)

    operator>>: input operator - gets input
    operator<<: output operator - pushes output 
*/

using namespace std;

int main() {
    // basic cout example
    bitset<8> s{ "01110011" };
    string str("Crying zeros and I'm hearing ");
    size_t num{ 111 };
    cout << s << '\n' << str << num << "s\n"; // output operations can be chained together

    // example of cout and cin
    // NOTE! If the input fails to convert, the stream enters a bad state and you can't use it anymore
    //  If something like std::cin gets into a bad state, you can run the following:
    //      - std::cin.clear() -- clears the error flags
    //      - std::cin.ignore() -- clear out the rest of the buffer to allow new input
    double x, y;
    cout << "X: ";
    cin >> x;
    cout << "Y: ";
    cin >> y;

    string op;
    cout << "Operation: ";
    cin >> op;
    if (op == "+") {
        cout << x + y;
    } else if (op == "-") {
        cout << x - y;
    } else if (op == "*") {
        cout << x * y;
    } else if (op == "/") {
        cout << x / y;
    } else {
        cout << "Unknown operation " << op;
    }

    cout << "\n";

    // streams implement a bool conversion that lets you know if the stream is in a good state for not
    // stream states:
    //      - goobit: stream is in a good working state
    //      - eofbit: stream encountered an EOF (end of file)
    //      - failbut: steam operation failed, but stream itself may still be good
    //      - badbit: catastrophic error on stream
    std::string word;
    size_t count{};
    while (std::cin >> word) {
        count++;
    }
    std::cout << "Discovered " << count << " words.\n";

    cin.clear(); // clears buffer of any remaining input before taking in more.

    // you can use exceptions() method to have a stream throw an error when a certain bit state is achieved
    cin.exceptions(istream::badbit);
    string word2;
    size_t count2{};
    try {
        while(cin >> word2) {
            count2++;
        }
        
    } catch (const std::exception& e) {
        cerr << "Error occurred reading from stdin: " << e.what();
    }

    /*
        Manipulators: objects that modify how streams should interpret or format output
        
        Manipulators provided by the Stdlib:
            - std::ws (<istream>): Skips over all whitespace
            - std::flush (<ostream>): writes any buffered data to the stream
            - std:: ends (<ostream>): sends a null byte
            - std::endl (<ostream>): sends a new line and flushes.

        Rule of thumb: use endl when you know you won't be writing any text out for a while, otherwise send '\n'
    */

    cout << "Discovered " << count2 << " words." << endl;

    /*
        String Streams: Basically string builders
    */
   std::ostringstream ss;
   ss << "By Grabathar's hammer, ";
   ss << "by the suns of Worvan, ";
   ss << "you shall be avenged.";

   cout << ss.str() << endl;

   /*
        File Streams: Provide RAII-wrapped facilities for reading and writing files
        Open Mode Exmamples:
            - std::ios::in - Read in file (must exist)
            - std::ios::out - Erase file then write (created if doesn't exist)
            - std::ios::app - Append to file (must exist)
            - in|out - Read and write from beginning of file (must exist)
            - in|app - Update at end (created if doesn't exist)
            - out|app - Append to file (created if doesn't exist)

        File streams fail silently, just like other streams. Can use the is_open() method
            to determine if a file successfully opened.
        Files use a buffer under the covers. The rdbuf() method can be used to dump the contents of
            of the file buffer to stdout.
   */
  std::ofstream numbers_out_file { "numbers.txt", std::ios::out|std::ios::app };
  numbers_out_file << -54 << endl;
  numbers_out_file << 203 << endl;
  numbers_out_file << 9000 << endl;
  numbers_out_file << 2345 << endl;

  numbers_out_file.close();

  std::ifstream numbers_in_file { "numbers.txt" };
  auto max_num = numeric_limits<int>::min();
  int value;
  while (numbers_in_file >> value) {
      max_num = max_num < value ? value : max_num;
  }

  cout << "Max number in file is " << max_num << endl;
}