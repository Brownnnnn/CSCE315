#include "./libs/DBMS.h"

int main(){
    DBMS app;
    app.init();
    while(true){
        try{
            app.options();
            return 0;
        }
        catch(runtime_error err){
            cout << err.what() << "\nTry Again? Enter y or n" << endl;
            char c;
            cin >> c;
            if(!cin || c == 'n') break;
            app.clear();
        }
    }

    return 0;
}