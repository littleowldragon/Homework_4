#include <raylib.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

// Struct to encapsulate our UI library
struct UiLibrary
{
    // ID of the currently hot widget
    int hot = -1;

    // ID of the currently active widget
    int active = -1;

    // ID of the previous active widget
    int prev_active = -1;
    Color color = Color{62, 181, 29, 255};
    // Creates a button with the specified text and bounds
    // Returns true if this button was clicked in this frame
    bool Button(int id, const std::string& text, const Rectangle& bounds)
    {
        bool result = false;

        // If this button is the currently active widget, that means
        // the user is currently interacting with this widget
        if (id == active)
        {
            // If the user released the mouse button while we are active,
            // register as a click
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                // Make sure that we are still the hot widget
                // (User might have dragged mouse to another widget, in which case we will not count as a button click)
                if (id == hot)
                {
                    result = true;
                }
                prev_active = active;
                // Set ourselves to not be active anymore
                active = -1;
            }
        }

        // If we are currently the hot widget
        if (id == hot)
        {
            // Color of hot button is Yellow 
            // If the user pressed the left mouse button, that means the user started
            // interacting with this widget, so we set this widget as active
            color = Color{152, 252, 124, 255};
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                active = id;
                color = Color{31, 89, 15, 255};
            }
            DrawRectangleRec(bounds, color);
        }
       
        // If the mouse cursor is hovering within our boundaries
        if (CheckCollisionPointRec(GetMousePosition(), bounds))
        {
            // Set this widget to be the hot widget
            hot = id;
        }
        // If the mouse cursor is not on top of this widget, and this widget
        // was previously the hot widget, set the hot widget to -1
        // We check if this widget was the previously hot widget since there is a possibility that
        // the hot widget is now a different widget, and we don't want to overwrite that with -1
        else if (hot == id)
        {
            hot = -1;
            prev_active = -1;
        }

        if ((id != hot && id != active) || hot == prev_active)
        {
            color = Color{62, 181, 29, 255};
            // Draw button if it's not hot (awww) and active (weeee)
            DrawRectangleRec(bounds, color);
        }

        DrawText(text.c_str(), bounds.x+10, bounds.y+10, 16, BLACK);

        return result;
    }

    bool Label(int id, const std::string& text, const Rectangle& bounds){
        bool result = false;
        // If this button is the currently active widget, that means
        // the user is currently interacting with this widget
        if (id == active)
        {
            // If the user released the mouse button while we are active,
            // register as a click
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                // Make sure that we are still the hot widget
                // (User might have dragged mouse to another widget, in which case we will not count as a button click)
                if (id == hot)
                {
                    result = true;
                }

                // Set ourselves to not be active anymore
                active = -1;
            }
        }

        // If we are currently the hot widget
        if (id == hot)
        {
            // Color of hot button is Yellow 
            // If the user pressed the left mouse button, that means the user started
            // interacting with this widget, so we set this widget as active
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                active = id;
            }
        }
       
        // If the mouse cursor is hovering within our boundaries
        if (CheckCollisionPointRec(GetMousePosition(), bounds))
        {
            // Set this widget to be the hot widget
            hot = id;
        }
        // If the mouse cursor is not on top of this widget, and this widget
        // was previously the hot widget, set the hot widget to -1
        // We check if this widget was the previously hot widget since there is a possibility that
        // the hot widget is now a different widget, and we don't want to overwrite that with -1
        else if (hot == id)
        {
            hot = -1;
        }

        DrawText(text.c_str(), bounds.x, bounds.y, 16, BLACK);
        return result;
    }
};

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);   
    int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Immediate Mode");
    SetTargetFPS(60);

    UiLibrary ui_library;
    bool label_0_checked = false;
    std::string label_text = "[ ]Lock Screen";
    while (!WindowShouldClose())
    {
        ClearBackground(WHITE);
        BeginDrawing();
        if (ui_library.Button(0, "800x600", { 10, 10, 80, 40 }))
        {
            if(IsWindowState(FLAG_WINDOW_RESIZABLE) == 1){
                SetWindowSize(800,600);
            }
        }

        if (ui_library.Button(1, "1280x720", { 100, 10, 80, 40 }))
        {
            if(IsWindowState(FLAG_WINDOW_RESIZABLE) == 1){
                SetWindowSize(1280,720);
            }
        }

        if (ui_library.Button(2, "1366x768", { 200, 10, 80, 40 }))
        {
            if(IsWindowState(FLAG_WINDOW_RESIZABLE) == 1){
                SetWindowSize(1366, 768);
            }
        }

        if (ui_library.Label(3, label_text, { 300, 10, 80, 40}))
        {
            if(label_0_checked == false){
                label_0_checked = true;
                ClearWindowState(FLAG_WINDOW_RESIZABLE);
                label_text = "[X]Lock Screen";
            }
            else{
                label_0_checked = false;
                SetConfigFlags(FLAG_WINDOW_RESIZABLE);
                label_text = "[ ]Lock Screen";
            }
        }
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
