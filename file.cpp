class fileHandler{
    private:
        const string prefix("d:\\base_main\\AlexRom\\final\\text ");
        const string extension(".txt");
        int CurrentFile;
    public:
        ifstream openNewFile(){
            ++CurrentFile;
            std::stringstream number;
            number << CurrentFile;
        }
        bool areMoreFiles() {
            return CurrentFile < 7146;
        }
        fileHandler(): CurrentFile(0) {
        };

}



    for(int i = 1 ; i <= 7146 ; ++i) {  //7146
        clock_t firstTime = clock();

        ifstream file((prefix + number.str() + extension).c_str()) ;
        while(!file.eof()) {
            string word;
            file >> word;
            Dict.add(word);
            //Dict.print();
        }
        cout << "Время обработки текста #" << i << " : " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << endl;
        file.close();
    }
