#include "pch.h"

#define INSIDE_MANAGED_CODE
#include "TerraBridge.h"
#include <vector>
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

using namespace std;

using namespace System::Runtime::InteropServices;
using namespace Microsoft;

#using <mscorlib.dll>
//#include "../Hack/Ultility.h"




//#using "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Terraria\\ReLogic.Native.dll"
#using "C:\\Windows\\Microsoft.Net\\assembly\\GAC_32\\Microsoft.Xna.Framework\\v4.0_4.0.0.0__842cf8be1de50553\\Microsoft.Xna.Framework.dll"
#using "C:\\Windows\\Microsoft.NET\\assembly\\GAC_32\\Microsoft.Xna.Framework.Game\\v4.0_4.0.0.0__842cf8be1de50553\\Microsoft.Xna.Framework.Game.dll"
#using "C:\\Windows\\Microsoft.Net\\assembly\\GAC_32\\Microsoft.Xna.Framework.Graphics\\v4.0_4.0.0.0__842cf8be1de50553\\Microsoft.Xna.Framework.Graphics.dll"
using namespace Microsoft::Xna::Framework;
using namespace Microsoft::Xna::Framework::Input;
using namespace Microsoft::Xna::Framework::Graphics;


using namespace System;
using namespace System::IO;

#using "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Terraria\\Terraria.exe"
//using namespace ReLogic;
using namespace Terraria;
using namespace Terraria::UI::Chat;
using namespace Terraria::UI::Gamepad;
using namespace Terraria::GameInput;
using namespace Terraria::GameContent;

string to_string(String^ csStr);
wstring to_wstring(String^ csStr);
wstring lowerCase(wstring str);
void ScreenTextAsm(TerraBridge::CLI_DrawColorCodedString args);


typedef Vector2 (*DrawString)();

double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

double distance(Entity^ ent1, Entity^ ent2) {
    return distance(ent1->position.X, ent1->position.Y, ent2->position.X, ent2->position.X);
}

void TerraBridge::EmptyFunc() {

}

bool TerraBridge::Test() {
    
    return false;
}

bool TerraBridge::Test(const wstring &args) {
    //CombatText::NewText(Microsoft::Xna::Framework::Rectangle(200, 200, 0, 0), Microsoft::Xna::Framework::Color(255, 0, 0), gcnew String(args.c_str()), false, false);
    //Microsoft::Xna::Framework::Graphics::SpriteFont^ font = Content::Load<Microsoft::Xna::Framework::Graphics::SpriteFont^>(gcnew String("Times New Roman"));

    //Main::spriteBatch->DrawString(nullptr, gcnew String(args.c_str()), Vector2(100, 100), Microsoft::Xna::Framework::Color(255, 0, 0));
    return false;
}



void TerraBridge::DrawCursor(int x, int y, const TerraBridge::Color& c, bool smart) {
    Microsoft::Xna::Framework::Color _color = Main::cursorColor;
    bool rainbow = Main::LocalPlayer->hasRainbowCursor;
    if (!Main::gameMenu && rainbow)
    {
        Main::cursorColor = Microsoft::Xna::Framework::Color(c.r, c.g, c.b, c.a);
        Main::LocalPlayer->hasRainbowCursor = false;
    }


    int _x = Main::mouseX, _y = Main::mouseY;
    Main::mouseX = x;
    Main::mouseY = y;
    Main::DrawCursor(Main::DrawThickCursor(smart), smart);
    
    

    Main::mouseX = _x;
    Main::mouseY = _y;
    if (rainbow) {
        Main::cursorColor = _color;
        Main::LocalPlayer->hasRainbowCursor = rainbow;
    }
}

void TerraBridge::GetCursorPos(int& x, int& y, TerraBridge::Color& c) {
    x = Main::mouseX, y = Main::mouseY;
    Microsoft::Xna::Framework::Color _c = Main::hslToRgb(Main::GlobalTimeWrappedHourly * 0.25 - (int)(Main::GlobalTimeWrappedHourly * 0.25), 1, 0.5f, byte::MaxValue);
    c.r = _c.R;
    c.g = _c.G;
    c.b = _c.B;
    c.a = _c.A;
}

bool TerraBridge::NeedCancelGrapple() {
    for (int i = 0; i < Main::projectile->Length; i++) {
        Projectile^ _p = Main::projectile[i];
        if (_p->active && _p->type == 403 && _p->owner == Main::myPlayer) {
            return false;
        }
    }
    return Main::LocalPlayer->grappling[0] >= 0;
}



void TerraBridge::UpdateInputs(const InputMap& inMap) {
    if (inMap.mouseLeft != SKIP) {
        PlayerInput::Triggers->Current->MouseLeft = (bool)inMap.mouseLeft;
        Main::mouseLeft = (bool)inMap.mouseLeft;
    }

    if (inMap.mouseRight != SKIP) {
        PlayerInput::Triggers->Current->MouseRight = (bool)inMap.mouseRight;
        Main::mouseRight = (bool)inMap.mouseRight;
    }



    if (inMap.up != SKIP)
        PlayerInput::Triggers->Current->Up = (bool)inMap.up;

    if (inMap.down != SKIP)
        PlayerInput::Triggers->Current->Down = (bool)inMap.down;

    if (inMap.left != SKIP)
        PlayerInput::Triggers->Current->Left = (bool)inMap.left;

    if (inMap.right != SKIP)
        PlayerInput::Triggers->Current->Right = (bool)inMap.right;



    if (inMap.jump != SKIP)
        PlayerInput::Triggers->Current->Jump = (bool)inMap.jump;

    if (inMap.grapple != SKIP)
        PlayerInput::Triggers->Current->Grapple = (bool)inMap.grapple;

    if (inMap.quickHeal != SKIP)
        PlayerInput::Triggers->Current->QuickHeal = (bool)inMap.quickHeal;

    if (inMap.quickBuff != SKIP)
        PlayerInput::Triggers->Current->QuickBuff = (bool)inMap.quickBuff;

    if (inMap.quickMana != SKIP)
        PlayerInput::Triggers->Current->QuickMana = (bool)inMap.quickMana;

    if (inMap.Hotbar >= 1 && inMap.Hotbar <= 10) {
        PlayerInput::Triggers->Current->Hotbar1 = false;
        PlayerInput::Triggers->Current->Hotbar2 = false;
        PlayerInput::Triggers->Current->Hotbar3 = false;
        PlayerInput::Triggers->Current->Hotbar4 = false;
        PlayerInput::Triggers->Current->Hotbar5 = false;
        PlayerInput::Triggers->Current->Hotbar6 = false;
        PlayerInput::Triggers->Current->Hotbar7 = false;
        PlayerInput::Triggers->Current->Hotbar8 = false;
        PlayerInput::Triggers->Current->Hotbar9 = false;
        PlayerInput::Triggers->Current->Hotbar10 = false;

        switch (inMap.Hotbar) {
        case 1:
            PlayerInput::Triggers->Current->Hotbar1 = true;
            break;
        case 2:
            PlayerInput::Triggers->Current->Hotbar2 = true;
            break;
        case 3:
            PlayerInput::Triggers->Current->Hotbar3 = true;
            break;
        case 4:
            PlayerInput::Triggers->Current->Hotbar4 = true;
            break;
        case 5:
            PlayerInput::Triggers->Current->Hotbar5 = true;
            break;
        case 6:
            PlayerInput::Triggers->Current->Hotbar6 = true;
            break;
        case 7:
            PlayerInput::Triggers->Current->Hotbar7 = true;
            break;
        case 8:
            PlayerInput::Triggers->Current->Hotbar8 = true;
            break;
        case 9:
            PlayerInput::Triggers->Current->Hotbar9 = true;
            break;
        case 10:
            PlayerInput::Triggers->Current->Hotbar10 = true;
            break;
        }
    }
}

void TerraBridge::UpdateMouse(int x, int y) {
    MouseState newSt = Mouse::GetState();
    int _x, _y;

    if (x == SKIP)
        _x = newSt.X;
    else
        _x = x;// (int)(x / PlayerInput::RawMouseScale.X);

    if (y == SKIP)
        _y = newSt.Y;
    else
        _y = y;// (int)(y / PlayerInput::RawMouseScale.Y);

    PlayerInput::MouseInfo = MouseState(_x, _y, newSt.ScrollWheelValue, newSt.LeftButton, newSt.MiddleButton, newSt.RightButton, newSt.XButton1, newSt.XButton2);
}



bool TerraBridge::CanUse() {
    return Main::hasFocus && !Main::drawingPlayerChat && !Main::editSign && !Main::editChest && !Main::blockInput;
}

void TerraBridge::Mana() {
    PlayerInput::Triggers->Current->QuickMana = true;
}

bool TerraBridge::NeedMana() {
    Player^ p = Main::LocalPlayer;
    Item^ i = p->inventory[p->selectedItem];
    return i->magic && p->statMana < i->mana && p->statMana < p->statManaMax2;
}

void TerraBridge::Buff() {
    //PlayerInput::Triggers->Current->QuickBuff = true;
    Main::LocalPlayer->QuickBuff();
}

bool TerraBridge::NeedBuff() {
    Player^ myPlayer = Main::LocalPlayer;
    if (myPlayer->cursed || myPlayer->CCed || myPlayer->dead || myPlayer->CountBuffs() == 22) {
        return false;
    }

    int bestFood = -1;

    bool flag = false;
    bool flag2 = false;
    for (int i = 0; i < 50; i++) {
        Item^ item = myPlayer->inventory[i];
        if (!item->IsAir && item->buffType > 0 && !item->summon) { // if the item has buff
            if (item->buffType == 26 || item->buffType == 206 || item->buffType == 207) { // if its a food
                if (item->buffType > bestFood) {
                    bestFood = item->buffType;
                }
            }
            else { // its a normal buff
                if (myPlayer->FindBuffIndex(item->buffType) == -1) // if the player has not that buff
                    flag = true;
            }
        }
    }

    if (bestFood != -1 && myPlayer->FindBuffIndex(bestFood) == -1) { // if the player is not buffed by the best food
        bool hasAFoodBuff = false;
        for (int i = 0; i < myPlayer->buffType->Length; i++)
            if (myPlayer->buffType[i] == 26 || myPlayer->buffType[i] == 206 || myPlayer->buffType[i] == 207) { // if the buff is from a food and its less than my best food
                hasAFoodBuff = true;
                if (myPlayer->buffType[i] < bestFood)
                    flag2 = true;
            }
        if (!hasAFoodBuff) {
            flag2 = true;
        }
    }
    return flag || flag2;
}

bool TerraBridge::IsAtMainMenu() {
    return Main::gameMenu;
}

const double dayLength = 54000.0;
const double nightLength = 32400.0;

bool TerraBridge::SetTime(double time) {
    //public static bool dayTime = true;
    //public static double time = 13500.0;
    if (time < 0 || time > nightLength + dayLength)
        return false;

    //Main::dayTime = time <= dayLength;
    //Main::time = (int)time % (int)(dayLength);

    Main::SkipToTime((int)time % (int)(dayLength), time <= dayLength);
    return true;
}



void TerraBridge::MessageOut(wstring& outMessage){
        
    System::String^ CSOutMessage = gcnew System::String(outMessage.c_str());
    Terraria::Chat::ChatMessage^ chatMessage = ChatManager::Commands->CreateOutgoingMessage(CSOutMessage);

    if (Terraria::Main::netMode == 1) {
        Terraria::Chat::ChatHelper::SendChatMessageFromClient(chatMessage);
    } else if (Terraria::Main::netMode == 0) {
        ChatManager::Commands->ProcessIncomingMessage(chatMessage, Main::myPlayer);
    }
}

void TerraBridge::LocalMessage(wstring& message, TerraBridge::Color color) {
    Microsoft::Xna::Framework::Color textColor = Microsoft::Xna::Framework::Color(color.r, color.g, color.b);
    System::String^ cs_str = gcnew System::String(message.c_str());
    int limit = -1;
    ((GameContent::UI::Chat::RemadeChatMonitor^)Terraria::Main::chatMonitor)->AddNewMessage(cs_str, textColor, limit);
}

bool TerraBridge::FillInventory() {
    bool _didOnce = false;
    for (int i = 0; i < Main::LocalPlayer->inventory->Length; i++) {
        Item^ item = Main::LocalPlayer->inventory[i];
        if (!item->IsAir > 0 && item->stack < item->maxStack) {
            item->stack = item->maxStack;
            _didOnce = true;
        }
    }
    return _didOnce;
}

bool TerraBridge::IsDrawingPlayerChat() {
    return Main::drawingPlayerChat;
}

void TerraBridge::GetCurrentChapAPIText(wstring& currentSTDText) {
    System::String^ currentCSText = Main::chatText;
    currentSTDText = to_wstring(currentCSText);
}

void TerraBridge::SetChatAPIText(wstring& text) {
    System::String^ csText = gcnew System::String(text.c_str());
    Main::chatText = csText;
}


string to_string(String^ csStr) {
    return msclr::interop::marshal_as<std::string>(csStr);
}
    
wstring to_wstring(String^ csStr) {
    return msclr::interop::marshal_as<std::wstring>(csStr);
}

wstring lowerCase(wstring str) {
    wstring ret = L"";
    int _diff = L'Z' - L'z';
    for (int i = 0; i < str.length(); i++) {
        if (str.at(i) <= L'Z' && str.at(i) >= L'A')
            ret += str.at(i) - _diff;
        else
            ret += str.at(i);
    }
    return ret;
}

string TerraBridge::hex_str(int num) {
    int digits = 1;
    while (pow(16, digits) < num)
        digits++;
    string hexStr = "";
    for (int i = digits; i >= 0; i--) {
        hexStr += hex_char((num / (int)pow(16, i)) % 16);
    }
    return hexStr;
}

char TerraBridge::hex_char(int num) {
    switch (num) {
    case 0:
        return '0';
    case 1:
        return '1';
    case 2:
        return '2';
    case 3:
        return '3';
    case 4:
        return '4';
    case 5:
        return '5';
    case 6:
        return '6';
    case 7:
        return '7';
    case 8:
        return '8';
    case 9:
        return '9';
    case 10:
        return 'A';
    case 11:
        return 'B';
    case 12:
        return 'C';
    case 13:
        return 'D';
    case 14:
        return 'E';
    case 15:
        return 'F';
    default:
        return '?';
    }
}

void TerraBridge::ScreenText(const wstring& text, const Color& c, int x, int y, DWORD DrawNetplayStatusTextAddress) {



    String^ csText = gcnew String(text.c_str());

    //Microsoft::Xna::Framework::Color textColor = Microsoft::Xna::Framework::Color(0xeb, 0xa5, 0x0f, 0xff);


    Microsoft::Xna::Framework::Color fontColor = Microsoft::Xna::Framework::Color(c.r, c.g, c.b);
    cli::array<TextSnippet^>^ textSnippet = ChatManager::ParseMessage(csText, fontColor)->ToArray();
    //baseColor
    Microsoft::Xna::Framework::Color csC = Microsoft::Xna::Framework::Color((int)Main::mouseTextColor, (int)Main::mouseTextColor, (int)Main::mouseTextColor, (int)Main::mouseTextColor);

    TerraBridge::UltilityData ultilityData = TerraBridge::UltilityData(DrawNetplayStatusTextAddress);
    CLI_DrawColorCodedString args;

    int returnVal = 0;
    Vector2  tmp = Vector2::Zero;
    Vector2 returnedVector = Vector2();

    {
        args.drawColorCodedString = ultilityData.DrawStringAddressPtr;
        args.spriteBatch = ultilityData.spriteBatch;
        args.font = *((DWORD*)(ultilityData.unknownEaxVal + 0x0c));
        args.snippets = (DWORD)&textSnippet;
        args.positionX = x;
        args.positionY = y;
        args.baseColor = (DWORD)&csC;
        args.rotation;
        args.origin;
        args.baseScale;
        args.hoveredSnippet = (DWORD)&returnVal;
        args.maxWidth;
        args.ignoreColors;
        args.returnVector = (DWORD)&returnedVector;
    };

    //ChatManager::DrawColorCodedString(Main::spriteBatch, FontAssets::MouseText->Value, textSnippet, Vector2(x, y), Microsoft::Xna::Framework::Color(c.r, c.g, c.b, 0), 0, Vector2::Zero, Vector2::One, returnVal, -1, false);
    //Main::spriteBatch->Begin(SpriteSortMode::Deferred, BlendState::AlphaBlend, Main::SamplerStateForCursor, nullptr, nullptr, nullptr, Main::UIScaleMatrix);
    ScreenTextAsm(args);
    //Main::spriteBatch->End();
    delete textSnippet;
}

#pragma unmanaged
void ScreenTextAsm(TerraBridge::CLI_DrawColorCodedString args) {
    DWORD gcAddress = args.returnVector;
    DWORD numAddress = args.hoveredSnippet;
    DWORD textSnippet = *((DWORD*)args.snippets);
    DWORD color = *((DWORD*)args.baseColor);

    __asm {
        pushad
        pushfd

        mov     esi, args.spriteBatch                       // SpiritBatch [047B5A00h]

        //mov     eax, args.font	                        // MouseText *this, [047B69ECh]
        //mov     ebx, [eax + 0Ch]
        //push    ebx	                                    // Mousetext->Value                 2nd  arg, Font
        push args.font

        push    textSnippet                                 // snippets                         3rd  arg, TextSnippet[]

        push	args.positionY                              // position                  		4th  arg, vector2->float
        push	args.positionX

        push    color                                       // color                            5th  arg, color

        push    00                                          // 0 or false                       6th  arg, float

        push	0x00000000                                  // vector.zero                      7th  arg, Vector2
        push	0x00000000

        push	0x3f800000                                  // vector.one                       8th  arg, Vector2
        push	0x3f800000

        push	numAddress                                  // num                              9th  arg, int

        push    0xBF800000	                                // -1                               10th arg, float

        push    00                                          // 0 or false                       11th arg, bool

        //lea changed to mov-------mov     eax, dword ptr[args.drawColorCodedString]
        mov     ecx, gcAddress                              // return address                   return, Vector2
        mov     edx, esi                                    // Main::spiritBatch                1st  arg, SpiritBatch

        mov     eax, dword ptr[args.drawColorCodedString]
        call[eax]                                       // drawColorCodedString(11 args)


        popfd
        popad
    }
}