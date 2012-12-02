#include <iostream>
#include <string>
#include <vector>
#include <utility>
//#include <boost/bimap.hpp>
#include <algorithm>
#include <boost/filesystem/fstream.hpp>

//using namespace std;
using std::vector;
using std::string;
using std::pair;
//using boost::filesystem;

class Dictionary {
    public:
        virtual int getNumber(string const &) {};
        virtual string getWord(int) {};
        virtual bool add (string const &) {};
//   virtual bool dump (void) {};
//   virtual bool load (void) {};
};

class Array : private Dictionary {
    private:
        vector<pair<int, string> > CodesWords;
        int renew(char *);
};

int Array::renew(char * filename) {
    return 1;
};

/*typedef boost::bimap< std::string, int > words_ids;
class BiMapDic : private Dictionary {
    words_ids wid;
    int getNumber(string const & word) {
        //translator::map_by<spanish>::const_iterator is = trans.by<spanish>().find(word);
        return wid.left.find(word)->get.right;

    }
};*/


int main() {
//    cout << "Hello world!" << endl;
    return 0;
}
