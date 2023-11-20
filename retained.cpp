#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <string>
#include <vector>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
//g++ retained.cpp -o out -I src/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm
// Generic UI component
struct UIComponent
{
    
    // Rectangle reprsenting the bounds of the UI component
    Rectangle bounds;

    // Draws this particular UI component
    // Set as abstract so that child widgets will implement this for us
    virtual void Draw() = 0;

    // Handles a mouse click event
    // Set as abstract so that child widgets will implement this for us
    // Returns a boolean indicating whether this UI component successfully handled the event
    virtual bool HandleClick(Vector2 click_position) = 0;
};

// Generic UI component that can contain other UI components as children
struct UIContainer : public UIComponent
{
    std::vector<UIComponent*> children;

    // Adds a child to the container
    void AddChild(UIComponent* child)
    {
        children.push_back(child);
    }

    // Draw
    void Draw() override
    {
        // Since we are just a container for other widgets, we simply
        // call the draw function of other widgets.
        // This results in a pre-order traversal when we also draw child widgets that are also containers
        for (size_t i = 0; i < children.size(); ++i)
        {
            children[i]->Draw();
        }
    }

    // Handles a mouse click event
    // Returns a boolean indicating whether this UI component successfully handled the event
    bool HandleClick(Vector2 click_position) override
    {
        // Since we are just a container for other widgets, we call the HandleClick function of our child widgets
        // Since later children are drawn last, we do the opposite of draw where we start from the last child.
        // This results in a pre-order traversal but in the reverse order.
        for (size_t i = children.size(); i > 0; --i)
        {
            // If a child already handles the click event, we instantly return so no more child widgets handle the click
            if (children[i - 1]->HandleClick(click_position))
            {
                return true;
            }
        }

        return false;
    }
};




struct Button : public UIComponent
{
    
    // Text displayed by the button
    std::string text;
    int width;
    int height;

    // Draw
    void Draw() override
    {
        DrawRectangleRec(bounds, GRAY);
        DrawText(text.c_str(), bounds.x, bounds.y, 14, BLACK);
    }


    // Handle mouse click
    // Returns a boolean indicating whether this UI component successfully handled the event
    bool HandleClick(Vector2 click_position) override
    {
        // Check if the mouse click position is within our bounds
        if (CheckCollisionPointRec(click_position, bounds))
        {

             std::cout << text << std::endl;  

             SetWindowSize(width, height);
            return true;
        }

        return false;
    }
};



// Button widget


// Text display widget
struct Label : public UIComponent
{
    // Text to be displayed
    std::string text;
    bool checked = false;
    // Draw
    void Draw() override
    {
        DrawText(text.c_str(), bounds.x, bounds.y, 14, BLACK);

    }

    // Handle mouse click
    // Returns a boolean indicating whether this UI component successfully handled the event
    bool HandleClick(Vector2 click_position) override
    {
        // Always return false since we're not going to handle click events for this particular widget
        // (unless you have to)
        if (CheckCollisionPointRec(click_position, bounds))
        {

            return true;
        }
        return false;
    }
};

// Struct to encapsulate our UI library
struct UILibrary
{
    // Root container
    UIContainer root_container;

    // Updates the current UI state
    void Update()
    {
        // If the left mouse button was released, we handle the click from the root container
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            root_container.HandleClick(GetMousePosition());
        }
    }

    // Draw
    void Draw()
    {
        root_container.Draw();
    }
};

int main()
{

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);    
    InitWindow(800, 600, "Retained Mode");
    SetTargetFPS(60);

    UILibrary ui_library;
    ui_library.root_container.bounds = { 10, 10, 600, 500 };

    Button button;
    button.text = "800x600";
    button.bounds = { 120, 10, 80, 40 };
    ui_library.root_container.AddChild(&button);
    button.width = 800;
    button.height= 600;

    Button button2;
    button2.text = "1280x720";
    button2.bounds = { 210, 10, 80, 40 };
    ui_library.root_container.AddChild(&button2);
    button2.width = 1280;
    button2.height= 720;

    Button button3;
    button3.text = "1366x768";
    button3.bounds = { 300, 10, 80, 40 };
    ui_library.root_container.AddChild(&button3);
    button2.width = 1366;
    button2.height= 768;

    Label label;
    label.text = "Resolution";
    label.bounds = { 10, 20, 100, 40 };
    ui_library.root_container.AddChild(&label);


    bool LockScreen = false;





    while (!WindowShouldClose())
    {
        ui_library.Update();

        ClearBackground(WHITE);
        BeginDrawing();
        ui_library.Draw();
         ClearBackground(RAYWHITE);

        


            if (LockScreen) DrawLine(500, 0, 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.6f));
            GuiCheckBox((Rectangle){ 10, 60, 30, 30 }, "Lock Screen", &LockScreen);


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
