#include <cstddef>
#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
    string input;
    HWND wesnoth_window = FindWindow(NULL, "The Battle for Wesnoth - 1.14.9");  //window name of the game/app
    if(wesnoth_window == NULL){
        cout << "Failed to find window\n";
        cin >> input;
        return 1;
    }
    else{
        cout << "Window found\n"; 
        DWORD process_id = 0;
        GetWindowThreadProcessId(wesnoth_window, &process_id);
        HANDLE wesnoth_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
        if (wesnoth_process == NULL) {
            cout << "Failed to open process\n";
            cin >> input;
            return 1;
        }
        else {
            cout << "Process opened successfully\n";
            SIZE_T Player_Base_Address = 0x017EECB8, Game_Base_Address = 0, Gold_Base_Address = 0, Gold_Address = 0;
            SIZE_T bytes_read = 0, bytes_written = 0;
            int gold; 
            ReadProcessMemory(wesnoth_process, (LPVOID)(Player_Base_Address + 0x60),  &Game_Base_Address, 4, &bytes_read);    //first layer pointer
            ReadProcessMemory(wesnoth_process, (LPVOID)(Game_Base_Address + 0xA90),  &Gold_Base_Address, 4, &bytes_read);   //second layer pointer
            Gold_Address = Gold_Base_Address + 4; //final offset to gold address
            cout << "Gold Address: " << uppercase << hex << Gold_Address << "\n";
            ReadProcessMemory(wesnoth_process, (LPVOID)Gold_Address,  &gold, 4, &bytes_read);  //reading gold and saving that in gold variable
            cout << "Gold: " << dec << gold << "\n";
            int new_gold;
            cout << "Enter New Gold Value(must be int)" << endl;
            cin >> new_gold;
            WriteProcessMemory(wesnoth_process, (LPVOID)Gold_Address, &new_gold, 4, &bytes_written); //writing gold from the new_gold variable
            cout << "Written Gold " << endl;
            ReadProcessMemory(wesnoth_process, (LPVOID)Gold_Address,  &gold, 4, &bytes_read);
            cout << "New Gold: " << dec << gold << "\n";
        }
    }    
    cin >> input;
    return 0;
}
