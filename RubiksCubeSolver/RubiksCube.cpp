#define _CRT_SECURE_NO_DEPRECATE
#include <iostream> 

using namespace std;

class RCube
{
public:
    RCube()
    {
        for (int i = 0; i < 9; i++)
        {
            white_[i] = 'w';
            red_[i] = 'r';
            orange_[i] = 'o';
            blue_[i] = 'b';
            green_[i] = 'g';
            yellow_[i] = 'y';
        }
        
    }

    RCube(const char* fname) {
        freopen(fname, "r", stdin);
        cout << "White Side : ";
        for (int i = 0; i < 9; ++i)
        {
            cin >> white_[i];
        }
        cout << "Red Side : ";
        for (int i = 0; i < 9; ++i)
        {
            cin >> red_[i];
        }
        cout << "Orange Side : ";
        for (int i = 0; i < 9; ++i)
        {
            cin >> orange_[i];
        }
        cout << "Blue Side : ";
        for (int i = 0; i < 9; ++i)
        {
            cin >> blue_[i];
        }
        cout << "Green Side : ";
        for (int i = 0; i < 9; ++i)
        {
            cin >> green_[i];
        }
        cout << "Yellow Side : ";
        for (int i = 0; i < 9; ++i)
        {
            cin >> yellow_[i];
        }
        fclose(stdin);
        freopen("CON", "r", stdin);
    }

    ~RCube() = default;

    void solve() {
        cout << "\n----------------------STEPS----------------------\n";

        white_cross();
        white_corners();
        middle_layer();
        yellow_cross();
        yellow_corners();
        yellow_corners_orientation();
        final_step();

        show_edges();

        char* fname = new char[] {"result.txt"};
        write_in_file(fname);
    }

    void show_edges() {
        cout << "\n--------------------EDGES------------------------\n";
        cout << "White Side : ";
        display_edge(white_); cout << endl << endl;
        cout << "Red Side : ";
        display_edge(red_); cout << endl << endl;
        cout << "Orange Side : ";
        display_edge(orange_); cout << endl << endl;
        cout << "Blue Side : ";
        display_edge(blue_); cout << endl << endl;
        cout << "Green Side : ";
        display_edge(green_); cout << endl << endl;
        cout << "Yellow Side : ";
        display_edge(yellow_); cout << endl << endl;
    }

    void display_edge(char edge[9]) {
        for (int i = 0; i < 9; i++)
            cout << edge[i];
        cout << endl;
    }

    char* generate_random_state() {
        cout << "\n-------------GENERATING-RANDOM-STATE-------------\n";
        for (int i = 0; i < 50; i++)
        {
            int edge = rand() % 6;
            switch (edge)
            {
            case 0:
                rotate_1('w'); break;
            case 1:
                rotate_1('y'); break;
            case 2:
                rotate_1('g'); break;
            case 3:
                rotate_1('b'); break;
            case 4:
                rotate_1('r'); break;
            case 5:
                rotate_1('o'); break;
            }
        }
        show_edges();
        char* fname = new char[] {"state0.txt"};
        write_in_file(fname);

        return fname;
    }

    void write_in_file(const char* fname) {
        freopen(fname, "w", stdout);
        display_edge(white_);
        display_edge(red_);
        display_edge(orange_);
        display_edge(blue_);
        display_edge(green_);
        display_edge(yellow_);
        fclose(stdout);
        freopen("CON", "w", stdout);    
    }

    void rotate_1(char color) {
        if (color == 'w')
        {
            cout << "White" << endl;
            swap(white_[7], white_[1]);
            swap(white_[7], white_[3]);
            swap(white_[7], white_[5]);
            swap(white_[0], white_[2]);
            swap(white_[0], white_[4]);
            swap(white_[0], white_[6]);

            swap(blue_[0], orange_[0]);
            swap(blue_[7], orange_[7]);
            swap(blue_[6], orange_[6]);
            swap(orange_[6], green_[6]);
            swap(orange_[7], green_[7]);
            swap(orange_[0], green_[0]);
            swap(green_[6], red_[6]);
            swap(green_[7], red_[7]);
            swap(green_[0], red_[0]);
        }

        else if (color == 'r')
        {
            cout << "Red" << endl;
            swap(red_[0], red_[2]);
            swap(red_[0], red_[4]);
            swap(red_[0], red_[6]);
            swap(red_[1], red_[3]);
            swap(red_[1], red_[5]);
            swap(red_[1], red_[7]);

            swap(blue_[6], white_[3]);
            swap(blue_[5], white_[2]);
            swap(blue_[4], white_[1]);
            swap(white_[1], green_[0]);
            swap(white_[2], green_[1]);
            swap(white_[3], green_[2]);
            swap(green_[0], yellow_[3]);
            swap(green_[1], yellow_[2]);
            swap(green_[2], yellow_[1]);

        }

        else if (color == 'y')
        {
            cout << "Yellow" << endl;
            swap(yellow_[1], yellow_[7]);
            swap(yellow_[1], yellow_[5]);
            swap(yellow_[1], yellow_[3]);
            swap(yellow_[0], yellow_[6]);
            swap(yellow_[0], yellow_[4]);
            swap(yellow_[0], yellow_[2]);

            swap(blue_[4], red_[4]);
            swap(blue_[3], red_[3]);
            swap(blue_[2], red_[2]);
            swap(red_[2], green_[2]);
            swap(red_[3], green_[3]);
            swap(red_[4], green_[4]);
            swap(green_[4], orange_[4]);
            swap(green_[3], orange_[3]);
            swap(green_[2], orange_[2]);

        }

        else if (color == 'o')
        {
            cout << "Orange" << endl;
            swap(orange_[0], orange_[2]);
            swap(orange_[0], orange_[4]);
            swap(orange_[0], orange_[6]);
            swap(orange_[1], orange_[3]);
            swap(orange_[1], orange_[5]);
            swap(orange_[1], orange_[7]);

            swap(blue_[2], yellow_[5]);
            swap(blue_[1], yellow_[6]);
            swap(blue_[0], yellow_[7]);
            swap(yellow_[5], green_[6]);
            swap(yellow_[6], green_[5]);
            swap(yellow_[7], green_[4]);
            swap(green_[6], white_[7]);
            swap(green_[5], white_[6]);
            swap(green_[4], white_[5]);

        }

        else if (color == 'g')
        {
            cout << "Green" << endl;
            swap(green_[0], green_[2]);
            swap(green_[0], green_[4]);
            swap(green_[0], green_[6]);
            swap(green_[1], green_[3]);
            swap(green_[1], green_[5]);
            swap(green_[1], green_[7]);

            swap(white_[5], orange_[2]);
            swap(white_[4], orange_[1]);
            swap(white_[3], orange_[0]);
            swap(yellow_[3], orange_[2]);
            swap(yellow_[4], orange_[1]);
            swap(yellow_[5], orange_[0]);
            swap(yellow_[3], red_[6]);
            swap(yellow_[4], red_[5]);
            swap(yellow_[5], red_[4]);

        }

        else if (color == 'b')
        {
            cout << "Blue" << endl;
            swap(blue_[0], blue_[2]);
            swap(blue_[0], blue_[4]);
            swap(blue_[0], blue_[6]);
            swap(blue_[1], blue_[3]);
            swap(blue_[1], blue_[5]);
            swap(blue_[1], blue_[7]);

            swap(yellow_[1], orange_[4]);
            swap(yellow_[0], orange_[5]);
            swap(yellow_[7], orange_[6]);
            swap(white_[7], orange_[4]);
            swap(white_[0], orange_[5]);
            swap(white_[1], orange_[6]);
            swap(white_[7], red_[0]);
            swap(white_[0], red_[1]);
            swap(white_[1], red_[2]);
        }
    }

    void rotate_2(char color) {
        if (color == 'w')
        {
            cout << "White'" << endl;
            swap(white_[1], white_[7]);
            swap(white_[1], white_[5]);
            swap(white_[1], white_[3]);
            swap(white_[0], white_[6]);
            swap(white_[0], white_[4]);
            swap(white_[0], white_[2]);

            swap(blue_[0], orange_[0]);
            swap(blue_[7], orange_[7]);
            swap(blue_[6], orange_[6]);
            swap(blue_[6], green_[6]);
            swap(blue_[7], green_[7]);
            swap(blue_[0], green_[0]);
            swap(blue_[6], red_[6]);
            swap(blue_[7], red_[7]);
            swap(blue_[0], red_[0]);
        }

        else if (color == 'r')
        {
            cout << "Red'" << endl;
            swap(red_[0], red_[6]);
            swap(red_[0], red_[4]);
            swap(red_[0], red_[2]);
            swap(red_[1], red_[7]);
            swap(red_[1], red_[5]);
            swap(red_[1], red_[3]);

            swap(blue_[6], white_[3]);
            swap(blue_[5], white_[2]);
            swap(blue_[4], white_[1]);
            swap(blue_[4], green_[0]);
            swap(blue_[5], green_[1]);
            swap(blue_[6], green_[2]);
            swap(blue_[4], yellow_[3]);
            swap(blue_[5], yellow_[2]);
            swap(blue_[6], yellow_[1]);

        }

        else if (color == 'y')
        {
            cout << "Yellow'" << endl;
            swap(yellow_[1], yellow_[3]);
            swap(yellow_[1], yellow_[5]);
            swap(yellow_[1], yellow_[7]);
            swap(yellow_[0], yellow_[2]);
            swap(yellow_[0], yellow_[4]);
            swap(yellow_[0], yellow_[6]);

            swap(blue_[4], red_[4]);
            swap(blue_[3], red_[3]);
            swap(blue_[2], red_[2]);
            swap(blue_[2], green_[2]);
            swap(blue_[3], green_[3]);
            swap(blue_[4], green_[4]);
            swap(blue_[4], orange_[4]);
            swap(blue_[3], orange_[3]);
            swap(blue_[2], orange_[2]);

        }

        else if (color == 'o')
        {
            cout << "Orange'" << endl;
            swap(orange_[0], orange_[6]);
            swap(orange_[0], orange_[4]);
            swap(orange_[0], orange_[2]);
            swap(orange_[1], orange_[7]);
            swap(orange_[1], orange_[5]);
            swap(orange_[1], orange_[3]);

            swap(blue_[2], yellow_[5]);
            swap(blue_[1], yellow_[6]);
            swap(blue_[0], yellow_[7]);
            swap(blue_[2], green_[6]);
            swap(blue_[1], green_[5]);
            swap(blue_[0], green_[4]);
            swap(blue_[2], white_[7]);
            swap(blue_[1], white_[6]);
            swap(blue_[0], white_[5]);

        }

        else if (color == 'g')
        {
            cout << "Green'" << endl;
            swap(green_[0], green_[6]);
            swap(green_[0], green_[4]);
            swap(green_[0], green_[2]);
            swap(green_[1], green_[7]);
            swap(green_[1], green_[5]);
            swap(green_[1], green_[3]);

            swap(white_[5], orange_[2]);
            swap(white_[4], orange_[1]);
            swap(white_[3], orange_[0]);
            swap(yellow_[5], white_[3]);
            swap(yellow_[4], white_[4]);
            swap(yellow_[3], white_[5]);
            swap(white_[3], red_[4]);
            swap(white_[4], red_[5]);
            swap(white_[5], red_[6]);
        }

        else if (color == 'b')
        {
            cout << "Blue'" << endl;
            swap(blue_[0], blue_[6]);
            swap(blue_[0], blue_[4]);
            swap(blue_[0], blue_[2]);
            swap(blue_[1], blue_[7]);
            swap(blue_[1], blue_[5]);
            swap(blue_[1], blue_[3]);

            swap(yellow_[1], orange_[4]);
            swap(yellow_[0], orange_[5]);
            swap(yellow_[7], orange_[6]);
            swap(white_[7], yellow_[1]);
            swap(white_[0], yellow_[0]);
            swap(white_[1], yellow_[7]);
            swap(yellow_[1], red_[0]);
            swap(yellow_[0], red_[1]);
            swap(yellow_[7], red_[2]);
        }
    }

    

private:
    void white_bottom(char q) {
        if ((yellow_[0] == 'w' && blue_[3] == q) || (yellow_[2] == 'w' && red_[3] == q) || (yellow_[4] == 'w' && green_[3] == q) || (yellow_[6] == 'w' && orange_[3] == q))
        {
            if (q == 'b')
            {
                while (blue_[3] != q || yellow_[0] != 'w')
                {
                    rotate_1('y');
                }
            }
            if (q == 'r')
            {
                while (red_[3] != q || yellow_[2] != 'w')
                {
                    rotate_1('y');
                }
                if (q != 'b')
                {
                    while (white_[0] != 'w' && blue_[7] != 'b')
                    {
                        rotate_1('w');
                    }
                }
            }
            if (q == 'g')
            {
                while (green_[3] != q || yellow_[4] != 'w')
                {
                    rotate_1('y');
                }
                if (q != 'b')
                {
                    while (white_[0] != 'w' && blue_[7] != 'b')
                    {
                        rotate_1('w');
                    }
                }
            }
            if (q == 'o')
            {
                while (orange_[3] != q || yellow_[6] != 'w')
                {
                    rotate_1('y');
                }
                if (q != 'b')
                {
                    while (white_[0] != 'w' && blue_[7] != 'b')
                    {
                        rotate_1('w');
                    }
                }
            }
            rotate_1(q); rotate_1(q);
        }
    }

    void right_alg(char a, char c) {
        rotate_2(a);
        rotate_1('y'); rotate_1(a); white_bottom(c);
    }

    void white_right(char q) {
        if (blue_[1] == 'w' || red_[1] == 'w' || green_[1] == 'w' || orange_[1] == 'w')
        {
            if (blue_[5] == q && red_[1] == 'w')
            {
                right_alg('b', q);
            }
            if (red_[5] == q && green_[1] == 'w')
            {
                right_alg('r', q);
            }
            if (green_[5] == q && orange_[1] == 'w')
            {
                right_alg('g', q);
            }
            if (orange_[5] == q && blue_[1] == 'w')
            {
                right_alg('o', q);
            }
        }
    }

    void left_alg(char a, char c) {
        rotate_1(a); rotate_1('y'); rotate_2(a);
        white_bottom(c);
    }

    void white_left(char q) {
        if (blue_[5] == 'w' || red_[5] == 'w' || green_[5] == 'w' || orange_[5] == 'w')
        {
            if (blue_[5] == 'w' && red_[1] == q)
            {
                left_alg('r', q);
            }
            if (red_[5] == 'w' && green_[1] == q)
            {
                left_alg('g', q);
            }
            if (green_[5] == 'w' && orange_[1] == q)
            {
                left_alg('o', q);
            }
            if (orange_[5] == 'w' && blue_[1] == q)
            {
                left_alg('b', q);
            }
        }
    }

    void top_alg(char a, char b, char c) {
        rotate_2(a);
        rotate_1('w'); rotate_1(b); rotate_1('w');
        rotate_1('w'); rotate_1('w'); white_bottom(c);
    }

    void white_top(char q) {
        if (blue_[7] == 'w' && white_[0] == q)
        {
            top_alg('b', 'r', q);
        }
        if (red_[7] == 'w' && white_[2] == q)
        {
            top_alg('r', 'g', q);
        }
        if (green_[7] == 'w' && white_[4] == q)
        {
            top_alg('g', 'o', q);
        }
        if (orange_[7] == 'w' && white_[6] == q)
        {
            top_alg('o', 'b', q);
        }
    }

    void inv_alg(char a, char b, char c) {
        rotate_1(a); rotate_1(b); rotate_2('y'); rotate_2(b); rotate_2(a); white_bottom(c);
    }

    void white_bottom_inverted(char q) {
        if (blue_[3] == 'w' || red_[3] == 'w' || green_[3] == 'w' || orange_[3] == 'w')
        {
            if (blue_[3] == 'w' && yellow_[0] == q)
            {
                inv_alg('b', 'r', q);
            }
            if (red_[3] == 'w' && yellow_[2] == q)
            {
                inv_alg('r', 'g', q);
            }
            if (green_[3] == 'w' && yellow_[4] == q)
            {
                inv_alg('g', 'o', q);
            }
            if (orange_[3] == 'w' && yellow_[6] == q)
            {
                inv_alg('o', 'b', q);
            }
        }
    }

    void white_cross() {
        char prefer[4] = { 'b','r','g','o' };
        for (int i = 0; i < 4; i++)
        {
            if (white_[0] == 'w' && blue_[7] == prefer[i]) { rotate_1('b'); }
            if (white_[2] == 'w' && red_[7] == prefer[i]) { rotate_1('r'); }
            if (white_[4] == 'w' && green_[7] == prefer[i]) { rotate_1('g'); }
            if (white_[6] == 'w' && orange_[7] == prefer[i]) { rotate_1('o'); }
            white_bottom(prefer[i]); white_bottom_inverted(prefer[i]); white_left(prefer[i]); white_right(prefer[i]); white_top(prefer[i]);
            if (i != 0)
            {
                while (blue_[7] != 'b') { rotate_1('w'); }
            }
            if (white_[0] == 'w' && white_[2] == 'w' && white_[4] == 'w' && white_[6] == 'w' && blue_[7] == 'b' && red_[7] == 'r' && green_[7] == 'g' && orange_[7] == 'o')
            {
                break;
            }
        }
    }

    void white_corners_alg_left() {
        rotate_2('b');
        rotate_2('y');
        rotate_1('b');
    }

    void white_corners_alg_right() {
        rotate_1('r');
        rotate_1('y');
        rotate_2('r');
    }

    void white_corners() {
        while (red_[0] != 'r' || red_[6] != 'r' || blue_[0] != 'b' || blue_[6] != 'b' || orange_[0] != 'o' || orange_[6] != 'o' || green_[0] != 'g' || green_[6] != 'g')
        {
            while (red_[7] != 'r')
            {
                rotate_1('w');
            }
            if (blue_[4] == 'w' || red_[4] == 'w' || green_[4] == 'w' || orange_[4] == 'w')
            {
                while (blue_[4] != 'w')
                {
                    rotate_1('y');
                }
                while (red_[2] != red_[7])
                {
                    rotate_1('w');
                }
                white_corners_alg_left();
                while (red_[7] != 'r')
                {
                    rotate_1('w');
                }
            }
            else if (blue_[2] == 'w' || red_[2] == 'w' || green_[2] == 'w' || orange_[2] == 'w')
            {
                while (red_[2] != 'w')
                {
                    rotate_1('y');
                }
                while (red_[7] != yellow_[1])
                {
                    rotate_1('w');
                }
                white_corners_alg_right();
                while (red_[7] != 'r')
                {
                    rotate_1('w');
                }
            }
            else if (yellow_[1] == 'w' || yellow_[3] == 'w' || yellow_[5] == 'w' || yellow_[7] == 'w')
            {
                while (yellow_[1] != 'w')
                {
                    rotate_1('y');
                }
                while (red_[2] != blue_[7])
                {
                    rotate_1('w');
                }
                rotate_2('b');
                rotate_1('y'); rotate_1('y');
                rotate_1('b');
                while (blue_[4] != 'w')
                {
                    rotate_1('y');
                }
                while (red_[2] != red_[7])
                {
                    rotate_1('w');
                }
                white_corners_alg_left();
                while (red_[7] != 'r')
                {
                    rotate_1('w');
                }
            }
            else
            {
                while (red_[7] == red_[0])
                {
                    rotate_1('w');
                }
                white_corners_alg_left();
                while (red_[7] != 'r')
                {
                    rotate_1('w');
                }
            }
        }
    }

    void middle_place_left_alg(char left, char center) {
        rotate_2('y');
        rotate_2(left);
        rotate_1('y');
        rotate_1(left);
        rotate_1('y');
        rotate_1(center);
        rotate_2('y');
        rotate_2(center);
    }

    void middle_place_right_alg(char center, char right) {
        rotate_1('y');
        rotate_1(right);
        rotate_2('y');
        rotate_2(right);
        rotate_2('y');
        rotate_2(center);
        rotate_1('y');
        rotate_1(center);
    }

    void middle_layer() {
        while (red_[5] != 'r' || red_[1] != 'r' || blue_[1] != 'b' || blue_[5] != 'b' || orange_[1] != 'o' || orange_[5] != 'o' || green_[1] != 'g' || green_[5] != 'g')
        {

            if ((orange_[1] != 'o' && green_[5] != 'g') && (orange_[1] != 'y' || green_[5] != 'y'))
            {
                while (green_[3] != 'y' && yellow_[4] != 'y')
                {
                    rotate_1('y');
                }
                middle_place_right_alg('g', 'o');
            }

            else if ((orange_[5] != 'o' && blue_[1] != 'b') && (orange_[5] != 'y' || blue_[1] != 'y'))
            {
                while (orange_[3] != 'y' && yellow_[6] != 'y')
                {
                    rotate_1('y');
                }
                middle_place_right_alg('o', 'b');
            }

            else if ((blue_[5] != 'b' && red_[1] != 'r') && (blue_[5] != 'y' || red_[1] != 'y'))
            {
                while (blue_[3] != 'y' && yellow_[0] != 'y')
                {
                    rotate_1('y');
                }
                middle_place_right_alg('b', 'r');
            }

            else if ((red_[5] != 'r' && green_[1] != 'g') && (red_[5] != 'y' || green_[1] != 'y'))
            {
                while (red_[3] != 'y' && yellow_[2] != 'y')
                {
                    rotate_1('y');
                }
                middle_place_right_alg('r', 'g');
            }

            while (red_[3] == 'y' || yellow_[2] == 'y')
            {
                rotate_1('y');
            }

            if (red_[3] == 'r' && yellow_[2] != 'y')
            {
                if (yellow_[2] == 'g')
                {
                    middle_place_right_alg('r', 'g');
                }
                else if (yellow_[2] = 'b')
                {
                    middle_place_left_alg('b', 'r');
                }
            }
            else if (red_[3] == 'b' && yellow_[2] != 'y')
            {
                rotate_1('y');
                if (yellow_[0] == 'r')
                {
                    middle_place_right_alg('b', 'r');
                }
                else if (yellow_[0] = 'o')
                {
                    middle_place_left_alg('o', 'b');
                }
            }
            else if (red_[3] == 'o' && yellow_[2] != 'y')
            {
                rotate_1('y'); rotate_1('y');
                if (yellow_[6] == 'b')
                {
                    middle_place_right_alg('o', 'b');
                }
                else if (yellow_[6] = 'g')
                {
                    middle_place_left_alg('g', 'o');
                }
            }
            else if (red_[3] == 'g' && yellow_[2] != 'y')
            {
                rotate_1('y'); rotate_1('y'); rotate_1('y');
                if (yellow_[4] == 'o')
                {
                    middle_place_right_alg('g', 'o');
                }
                else if (yellow_[4] = 'r')
                {
                    middle_place_left_alg('r', 'g');
                }
            }
        }
    }

    void yellow_cross_algorithm() {
        rotate_1('r');
        rotate_1('y');
        rotate_1('g');
        rotate_2('y');
        rotate_2('g');
        rotate_2('r');
    }

    void yellow_cross() {
        while (yellow_[0] != 'y' || yellow_[2] != 'y' || yellow_[4] != 'y' || yellow_[6] != 'y')
        {
            if ((yellow_[0] == 'y' && yellow_[6] == 'y') || (yellow_[4] == 'y' && yellow_[6] == 'y')
                || (yellow_[2] == 'y' && yellow_[4] == 'y') || (yellow_[0] == 'y' && yellow_[2] == 'y'))
            {
                while (yellow_[0] != 'y' && yellow_[6] != 'y')
                {
                    rotate_1('y');
                }
                yellow_cross_algorithm();
            }
            if ((yellow_[2] == 'y' && yellow_[6] == 'y') || (yellow_[0] == 'y' && yellow_[4] == 'y'))
            {
                while (yellow_[0] != 'y' && yellow_[4] != 'y')
                {
                    rotate_1('y');
                }
                yellow_cross_algorithm();
                yellow_cross_algorithm();
            }
            else if (yellow_[8] == 'y')
            {
                yellow_cross_algorithm();
                rotate_1('y');
                yellow_cross_algorithm();
                yellow_cross_algorithm();
            }
        }
    }

    void yellow_corners_algorithm() {
        rotate_1('g');
        rotate_1('y');
        rotate_2('g');
        rotate_1('y');
        rotate_1('g');
        rotate_1('y');
        rotate_1('y');
        rotate_2('g');
    }

    void yellow_corners() {
        while (yellow_[1] != 'y' || yellow_[3] != 'y' || yellow_[5] != 'y' || yellow_[7] != 'y')
        {
            if ((yellow_[1] == 'y' && yellow_[3] != 'y' && yellow_[5] != 'y' && yellow_[7] != 'y')
                || (yellow_[3] == 'y' && yellow_[1] != 'y' && yellow_[5] != 'y' && yellow_[7] != 'y')
                || (yellow_[5] == 'y' && yellow_[1] != 'y' && yellow_[3] != 'y' && yellow_[7] != 'y')
                || (yellow_[7] == 'y' && yellow_[1] != 'y' && yellow_[3] != 'y' && yellow_[5] != 'y'))
            {
                while (yellow_[1] != 'y')
                {
                    rotate_1('y');
                }
                yellow_corners_algorithm();
            }
            else if ((green_[2] == 'y' && green_[4] == 'y' && yellow_[1] == 'y' && yellow_[7] == 'y')
                || (orange_[2] == 'y' && orange_[4] == 'y' && yellow_[1] == 'y' && yellow_[3] == 'y')
                || (blue_[2] == 'y' && blue_[4] == 'y' && yellow_[3] == 'y' && yellow_[5] == 'y')
                || (red_[2] == 'y' && red_[4] == 'y' && yellow_[5] == 'y' && yellow_[7] == 'y'))
            {
                while (red_[2] != 'y' && red_[4] != 'y' && yellow_[5] != 'y' && yellow_[7] != 'y')
                {
                    rotate_1('y');
                }
                yellow_corners_algorithm();
            }
            else if ((red_[4] == 'y' && orange_[2] == 'y' && yellow_[1] == 'y' && yellow_[7] == 'y')
                || (blue_[2] == 'y' && green_[4] == 'y' && yellow_[1] == 'y' && yellow_[3] == 'y')
                || (red_[2] == 'y' && orange_[4] == 'y' && yellow_[3] == 'y' && yellow_[5] == 'y')
                || (blue_[4] == 'y' && green_[2] == 'y' && yellow_[5] == 'y' && yellow_[7] == 'y'))
            {
                while (red_[2] != 'y' && orange_[4] != 'y' && yellow_[3] != 'y' && yellow_[5] != 'y')
                {
                    rotate_1('y');
                }
                yellow_corners_algorithm();
            }
            else if ((green_[2] == 'y' && green_[4] == 'y' && blue_[2] == 'y' && blue_[4] == 'y')
                || (red_[2] == 'y' && red_[4] == 'y' && orange_[2] == 'y' && orange_[4] == 'y'))
            {
                while (green_[2] != 'y' && green_[4] != 'y' && blue_[2] != 'y' && blue_[4] != 'y')
                {
                    rotate_1('y');
                }
                yellow_corners_algorithm();
            }
            else if ((green_[2] == 'y' && orange_[2] == 'y' && orange_[4] == 'y' && blue_[4] == 'y')
                || (red_[4] == 'y' && orange_[2] == 'y' && blue_[2] == 'y' && blue_[4] == 'y')
                || (red_[2] == 'y' && red_[4] == 'y' && green_[4] == 'y' && blue_[2] == 'y')
                || (green_[2] == 'y' && green_[4] == 'y' && orange_[4] == 'y' && red_[2] == 'y'))
            {
                while (green_[2] != 'y' && orange_[2] != 'y' && orange_[4] != 'y' && blue_[4] != 'y')
                {
                    rotate_1('y');
                }
                yellow_corners_algorithm();
            }
            else if ((yellow_[1] == 'y' && yellow_[5] == 'y' && yellow_[3] != 'y' && yellow_[7] != 'y')
                || (yellow_[3] == 'y' && yellow_[7] == 'y' && yellow_[1] != 'y' && yellow_[5] != 'y'))
            {
                while (red_[2] != 'y' && green_[4] != 'y')
                {
                    rotate_1('y');
                }
                yellow_corners_algorithm();
            }
        }
    }

    void yellow_corner_orientation_algorithm() {
        rotate_2('g');
        rotate_1('r');
        rotate_2('g');
        rotate_1('o'); rotate_1('o');
        rotate_1('g');
        rotate_2('r');
        rotate_2('g');
        rotate_1('o'); rotate_1('o');
        rotate_1('g'); rotate_1('g');
        rotate_2('y');
    }

    void yellow_corners_orientation() {
        while (red_[2] != 'r' || red_[4] != 'r' || green_[2] != 'g' || green_[4] != 'g'
            || orange_[2] != 'o' || orange_[4] != 'o' || blue_[2] != 'b' || blue_[4] != 'b')
        {
            if ((red_[2] == red_[4]) || (green_[2] == green_[4]) || (orange_[2] == orange_[4]) || (blue_[2] == blue_[4]))
            {
                while (orange_[2] != orange_[4])
                {
                    rotate_1('y');
                }
                yellow_corner_orientation_algorithm();
                while (blue_[2] != 'b')
                {
                    rotate_1('y');
                }
            }
            else
            {
                while (orange_[4] != 'o' && red_[4] != 'r')
                {
                    rotate_1('y');
                }
                yellow_corner_orientation_algorithm();
                while (orange_[2] != orange_[4])
                {
                    rotate_1('y');
                }
                yellow_corner_orientation_algorithm();
                while (blue_[2] != 'b')
                {
                    rotate_1('y');
                }
            }
        }
    }

    void yellow_edges_colour_arrangement_right() {
        rotate_1('r'); rotate_1('r');
        rotate_2('y');
        rotate_2('g');
        rotate_1('b');
        rotate_1('r'); rotate_1('r');
        rotate_2('b');
        rotate_1('g');
        rotate_2('y');
        rotate_1('r'); rotate_1('r');
    }

    void yellow_edges_colour_arrangement_left() {
        rotate_1('r'); rotate_1('r');
        rotate_1('y');
        rotate_1('b');
        rotate_2('g');
        rotate_1('r'); rotate_1('r');
        rotate_2('b');
        rotate_1('g');
        rotate_1('y');
        rotate_1('r'); rotate_1('r');
    }

    void final_step() {
        while (red_[2] != 'r')
        {
            rotate_1('r');
        }
        if (red_[3] == 'o' && orange_[3] == 'r' && blue_[3] == 'g' && green_[3] == 'b')
        {
            yellow_edges_colour_arrangement_left();
        }
        else if (red_[3] == 'b' && blue_[3] == 'r')
        {
            yellow_edges_colour_arrangement_left();
        }
        else if (red_[3] == 'g' && green_[3] == 'r')
        {
            yellow_edges_colour_arrangement_left();
        }
        while (orange_[2] != orange_[3])
        {
            rotate_1('y');
        }
        if (red_[3] == green_[2])
        {
            yellow_edges_colour_arrangement_right();
        }
        else if (red_[3] == blue_[2])
        {
            yellow_edges_colour_arrangement_left();
        }
        while (red_[3] != 'r')
        {
            rotate_1('y');
        }
    }

    char white_[9];
    char red_[9];
    char orange_[9];
    char blue_[9];
    char green_[9];
    char yellow_[9];
};


int main() {
    RCube rc;
    rc.generate_random_state();
    rc.solve();
    return 0;
}