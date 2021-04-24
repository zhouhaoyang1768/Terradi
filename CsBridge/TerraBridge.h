
#pragma once
#include <string>
#include <vector>
#include "Windows.h"
#include <locale>
#include <codecvt>

#ifdef INSIDE_MANAGED_CODE
#define DECLSPECIFIER __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define DECLSPECIFIER __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif



using namespace std;



class DECLSPECIFIER TerraBridge
{

private:
public:

#define PLAYER 0x00000001

#define SKIP -1

#define TEST -2
#define NO_PROBLEM -1
#define NO_TARGET 0
#define INVALID_WEAPON 1
    struct Color {
        int r, g, b, a;
        Color() : Color(0, 0, 0, 0) {};

        Color(int r, int g, int b) {
            this->r = r;
            this->g = g;
            this->b = b;
            a = 0;
        }

        Color(int r, int g, int b, int a) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        bool operator==(const Color& o) {
            return r == o.r && g == o.g && b == o.b && a == o.a;
        }
        bool operator!=(const Color& o) {
            return !(*this == o);
        }
    };
    struct InputMap {
        int mouseLeft = SKIP;
        int mouseRight = SKIP;

        int mouseX = SKIP;
        int mouseY = SKIP;

        int left = SKIP;
        int right = SKIP;
        int up = SKIP;
        int down = SKIP;

        int jump = SKIP;
        int grapple = SKIP;
        int quickHeal = SKIP;
        int quickMana = SKIP;
        int quickBuff = SKIP;

        int Hotbar = SKIP;

        InputMap() {

        }
    };
    struct EntityInfo {
        wstring name;

        float x;
        float y;

        float speedX;
        float speedY;

        int type;

        bool canHit;
    };
    struct UltilityData {
        DWORD spriteBatch;
        DWORD unknownEaxVal;
        DWORD unknownEdiVal;
        DWORD DrawStringAddressPtr;

        static wstring to_wstring(string s) {
            wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(s);
        }

        UltilityData(DWORD DrawNetplayStatusTextAddress) {
            //Color c = Color(0, 255, 0);
            spriteBatch = *(DWORD*)*(DWORD*)(DrawNetplayStatusTextAddress + 0x2D0 + 2);
            //wstring str = L"MouseTextValue: " + to_wstring(hex_str(MouseTextValue));
            //Message(str, c);

            unknownEaxVal = *(DWORD*)*(DWORD*)(DrawNetplayStatusTextAddress + 0x2D6 + 1);
            //str = L"unknownEaxVal: " + to_wstring(hex_str(unknownEaxVal));
            //Message(str, c);

            unknownEdiVal = *(DWORD*)*(DWORD*)(DrawNetplayStatusTextAddress + 0x2E3 + 2);
            //str = L"unknownEdiVal: " + to_wstring(hex_str(unknownEdiVal));
            //Message(str, c);

            DrawStringAddressPtr = *(DWORD*)(DrawNetplayStatusTextAddress + 0x34E + 2);
            //str = L"DrawStringAddressPtr: " + to_wstring(hex_str(DrawStringAddressPtr));
            //Message(str, c);
        }
    };
    
    typedef struct CLI_DrawColorCodedString {
        DWORD drawColorCodedString;
        DWORD spriteBatch;
        DWORD font;
        DWORD snippets;
        float positionX;
        float positionY;
        DWORD baseColor;
        DWORD rotation;
        DWORD origin;
        DWORD baseScale;
        DWORD hoveredSnippet;
        DWORD maxWidth;
        DWORD ignoreColors;
        DWORD returnVector;
    } CLI_DrawColorCodedString;

    
    

    static void EmptyFunc();

    static bool Test();
    static bool Test(const wstring &args);

    static void DrawCursor(int x, int y, const TerraBridge::Color& c, bool smart);
    static void GetCursorPos(int& x, int& y, TerraBridge::Color& c);
    static bool NeedCancelGrapple();
    
    static void UpdateInputs(const InputMap &inMap);
    static void UpdateMouse(int x, int y);
    
    static bool CanUse();
    static void Mana();
    static bool NeedMana();
    static void Buff();
    static bool NeedBuff();
    static bool IsAtMainMenu();
    static bool SetTime(double time);
    
    static void Teleport(const wstring& player, wstring &returnMessage);
    static void TeleportPos(float x, float y);
    
    static bool FillInventory();

    static void MessageOut(wstring &outMessage);
    static void LocalMessage(wstring& message, TerraBridge::Color color);
    static bool IsDrawingPlayerChat();
    static void GetCurrentChapAPIText(wstring& currentSTDText);
    static void SetChatAPIText(wstring& text);

    static void ScreenText(const wstring& text, const Color& c, int x, int y, DWORD DrawNetplayStatusTextAddress);


    static string hex_str(int num);
    static char hex_char(int num);
};