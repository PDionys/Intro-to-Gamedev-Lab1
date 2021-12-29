// версія 0.9.
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
using namespace std;
string win="";
// функція для очищення екрана
void clear_screen()
{
#if defined(WIN32)||defined(_WIN32)
    system("cls");
#else
    system("clear");
#endif
}
int odd(int number)
{
  if (number%2==0) return 0;
  else return 1;
}
class Model
{
  public:
  bool player;                 // true - human; false - computer
  //char pawn_field    [9][9];
  //char fence_horizont[8][8];
  //char fence_vertical[8][8];

  char total_field [17][17];
  int player1_coordinates[2];
  int player2_coordinates[2];
  int player1_fences=10;
  int player2_fences=10;
  char player1_symbol='H';
  char player2_symbol='C';
  char fence_horizont_symbol='+';
  char fence_vertical_symbol='+';
  Model()
  {
    player= true;
    player1_coordinates[0]=16;
    player1_coordinates[1]=8;
    player2_coordinates[0]=0; // на початку має бути 0
    player2_coordinates[1]=8;
    for (int i=0; i<17; i++)
    {
      for (int j=0; j<17; j++)
      {
        if (i==player2_coordinates[0] && j==player2_coordinates[1])
        {
          total_field[i][j]=player2_symbol;
        }
        else if (i==player1_coordinates[0] && j==player1_coordinates[1])
        {
          total_field[i][j]=player1_symbol;
        }
        else
          {total_field[i][j]=' ';}
      }
    }
  }
};
class View
{
  public:
  static void no_way()
  {
    cout<<"Стіна повністю блокує прохід"<<endl;
  }
  static void draw_field(Model &model)
  { 
    clear_screen(); // додав очистку екрана
    cout<<"У комп'ютера є "<<model.player2_fences<<" огорож. ";
    cout<<"У людини є "<<model.player1_fences<<" огорож."<<endl;
    for (int i=0; i<17; i++)
      if(odd(i))
        {cout<<i/2+1;}
      else
        {cout<<" ";}
    {cout<<endl;}
    for (int i=0; i<17; i++)
      {cout<<"=";}
    cout<<endl;
    for (int i=0; i<17; i++)
    {
      for (int j=0; j<17; j++)
      {
        cout<<model.total_field[i][j];
      }
      if(odd(i))
        {cout<<"= "<<i/2+1<<endl;}
      else cout<<"="<<endl;
      //cout<<"="<<endl;
    }
    for (int i=0; i<17; i++)
      {cout<<"=";}
    cout<<endl;
  };
};
class WayControl
{
  public:
    int player1_x;
    int player1_y;
    int player2_x;
    int player2_y;
    int player1_finish;
    int player2_finish;
    int rotation[2]={2,0};
    int direction=6;
    int total_field[17][17];
  static bool found(WayControl model, bool player)
  {
    if (player)
     return model.player1_y==0;
    else
     return model.player2_y==16;
  }
  WayControl(Model model)
  {
    player1_x=model.player1_coordinates[1];
    player1_y=model.player1_coordinates[0];
    player2_x=model.player2_coordinates[1];
    player2_y=model.player2_coordinates[0];
    player1_finish =0;
    player2_finish =16;
    for (int i=0; i<17; i++)
    {
      for (int j=0; j<17; j++)
      {
        if (model.total_field[i][j]=='|' || model.total_field[i][j]=='_')
          {total_field[i][j]=1;}
        else
          {total_field[i][j]=0;}
      }
    }
  }
  static void print_way(WayControl model, int x, int y)
  {
    clear_screen();
    for (int i=0; i<17; i++)
    {
      for (int j=0; j<17; j++)
      {
        if (i==y && j==x)
         cout<<"2";
        else if (model.total_field[i][j]==1)
          cout<<"1";
        else
          cout<<"0";
      }
      cout<<endl;
    }
    char key;
    cin>>key;
  }
  bool go(WayControl &model)
  {
    int x,y;
    x=model.player1_x+model.rotation[0];
    y=model.player1_y+model.rotation[1];
    if (x<0 || y<0 || x>16 || y>16 )
    {
      return false;
    }
    switch (model.direction)
    {
      case 2:
        if (model.total_field[y-1][x]==1)
          {return false;}
        break;
      case 4:
        if (model.total_field[y][x+1]==1)
          {return false;}
        break;
      case 6:
        if (model.total_field[y][x-1]==1)
          {return false;}
        break;  
      case 8:
        if (model.total_field[y+1][x]==1)
          {return false;}
        break;  
    }
    model.player1_x=x;
    model.player1_y=y;
    // для перевірки правильності можна вивести поточну модель
    //print_way(model, x, y);
    return true;
  }
  WayControl turn_left(WayControl &model)
  {
    if (model.rotation[0]==2 && model.rotation[1]==0)
      {model.rotation[0]=0; model.rotation[1]=-2;
       model.direction=8;
      }
    if (model.rotation[0]==0 && model.rotation[1]==-2)
      {model.rotation[0]=-2; model.rotation[1]=-0;
      model.direction=4;}
    if (model.rotation[0]==-2 && model.rotation[1]==0)
      {model.rotation[0]=0; model.rotation[1]=2;
      model.direction=2;
      }
    if (model.rotation[0]==0 && model.rotation[1]==2)
      {model.rotation[0]=2; model.rotation[1]=0;
      model.direction=6;}
    return model;
  }
  WayControl turn_right(WayControl &model)
  {
    if (model.rotation[0]==2 && model.rotation[1]==0)
      {model.rotation[0]=0; model.rotation[1]=2;
      model.direction=2;
      }
    if (model.rotation[0]==0 && model.rotation[1]==-2)
      {model.rotation[0]=2; model.rotation[1]=-0;
      model.direction=6;}
    if (model.rotation[0]==-2 && model.rotation[1]==0)
      {model.rotation[0]=0; model.rotation[1]=-2;
      model.direction=8;}
    if (model.rotation[0]==0 && model.rotation[1]==2)
      {model.rotation[0]=-2; model.rotation[1]=0;
      model.direction=4;}
    return model;
  }
  static bool possible_way(WayControl &way_model)
  {
    int count=0,key;
    bool znaideno1=found(way_model, 1);
    //print_way(way_model, way_model.player1_x, way_model.player1_y);
    while(!znaideno1)
    {
      //cout<<way_model.player1_x<<" "<<way_model.player1_y<<endl;
      int povorot, perehid, direct=rand()%3;
      if (direct==0) 
        {way_model.turn_left(way_model);}
      if (direct==1) 
        {way_model.turn_right(way_model);}
      if (direct==2) 
        {
          int coordinates[]={way_model.player1_x, way_model.player1_x};
        }
      perehid=way_model.go(way_model);
      //print_way(way_model, way_model.player1_x, way_model.player1_y);
      znaideno1=found(way_model, 1);
      count++;
      if (count ==2000000)
       return false;
    }
    return true;
  }
};
class Control
{
  public: 

  static void choice_revision(int choice, bool &player, Model &model, int x,int y,int x_opponent,int y_opponent)
  {
        int wall, opponent, wall_is_present,direction;
        switch (choice)
        {
          case 2:
            opponent=(y_opponent-y==2 && x==x_opponent)?true:false;
            wall    =(model.total_field[y+1][x]=='_')?true:false;
            if (y==16) 
              {
                if(player) cout<<"Ходу нема. Зробіть інший вибір";
                else {}
              }
            else if(wall)
              {
                if(player) cout<<"Там стіна. Ходу нема. Зробіть інший вибір";
              }
            else if(opponent)
              {
                wall    =(model.total_field[y+3][x]=='_')?true:false;
                if (y<14 && !wall)
                {
                  if(player)
                  {
                    model.total_field[y][x]=' ';
                    model.player1_coordinates[0]+=4;
                    y+=4;
                    model.total_field[y][x]='H';
                    View::draw_field(model);
                    player = !player;
                  }
                  else
                  {
                    model.total_field[y][x]=' ';
                    model.player2_coordinates[0]+=4;
                    y+=4;
                    model.total_field[y][x]='C';
                    View::draw_field(model);
                    player = !player;
                  }
                }
                else if (y<14 && wall)
                {
                  model.total_field[y][x]=' ';
                  int side_wall, left_wall, right_wall;
                  left_wall =(model.total_field[y_opponent][x_opponent-1]=='|')?1:0;
                  right_wall=(model.total_field[y_opponent][x_opponent+1]=='|')?2:0;
                  side_wall= left_wall + right_wall;
                  
                  switch (side_wall)
                  {
                    case 0:
                      if(player)
                      {
                        cout<<"Виберіть, куди ставити свою шашку: ліворуч (4) чи праворуч (6) від шашки опонента?"<<endl;
                        cin >>direction;
                        if     (direction==4) {  model.player1_coordinates[0]+=2; model.player1_coordinates[1]-=2; player = !player;}
                        else if(direction==6) {  model.player1_coordinates[0]+=2; model.player1_coordinates[1]+=2; player = !player;}
                        else  
                        {  
                          cout<<"Ви вибрали неіснуючий напрям, тому шашка встановлено зліва."<<endl;
                          model.player1_coordinates[0]+=2; model.player1_coordinates[1]-=2;
                          player = !player;
                        }
                      }
                      else
                      {
                        direction=4+rand()%2*2;
                        if     (direction==4) {  model.player2_coordinates[0]+=2; model.player2_coordinates[1]-=2; player = !player;}
                        else if(direction==6) {  model.player2_coordinates[0]+=2; model.player2_coordinates[1]+=2; player = !player;}
                      }
                      break;
                      case 1:
                      if(player)
                      {
                        model.player1_coordinates[0]+=2; model.player1_coordinates[1]+=2;
                        player = !player;
                      }
                      else
                      {
                        model.player2_coordinates[0]+=2; model.player2_coordinates[1]+=2;
                        player = !player;
                      }
                      break;
                    case 2:
                      if(player)
                      {
                        model.player1_coordinates[0]+=2; model.player1_coordinates[1]-=2;
                        player = !player;
                      }
                      else
                      {
                        model.player2_coordinates[0]+=2; model.player2_coordinates[1]-=2;
                        player = !player;
                      }
                      break;
                    case 3:
                      if(player)
                      {
                        if(player) cout<<"Перескок неможливий. Виберіть інший хід."<<endl;
                        std::this_thread::sleep_for (std::chrono::seconds(3));
                      }
                      break;
                    default:
                      break;
                  }
                  if(player)
                  {
                    model.total_field[model.player1_coordinates[0]][model.player1_coordinates[1]]='H';
                    View::draw_field(model);
                  }
                  else
                  {
                    model.total_field[model.player2_coordinates[0]][model.player2_coordinates[1]]='C';
                    View::draw_field(model);
                  }
                }
                else if (y==14)
                {
                  if(player)
                  {
                    model.total_field[y][x]=' ';
                    model.player1_coordinates[0]+=2;
                    model.player1_coordinates[1]+=2;
                    y+=2;
                    x+=2;
                    model.total_field[y][x]='H';
                    View::draw_field(model);
                    player = !player;
                  }
                  else
                  {
                    model.total_field[y][x]=' ';
                    model.player2_coordinates[0]+=2;
                    model.player2_coordinates[1]+=2;
                    y+=2;
                    x+=2;
                    model.total_field[y][x]='C';
                    View::draw_field(model);
                    player = !player;
                  }
                }
              }
            else 
              {
                if(player)
                {
                  model.total_field[y][x]=' ';
                  model.player1_coordinates[0]+=2;
                  y += 2;
                  model.total_field[y][x]='H';
                  View::draw_field(model);
                  player = !player;
                }
                else 
                {
                  model.total_field[y][x]=' ';
                  model.player2_coordinates[0]+=2;
                  y += 2;
                  model.total_field[y][x]='C';
                  View::draw_field(model);
                  player = !player;
                }
              }
            break;
          case 4: 
            opponent=(x-x_opponent==2 && y==y_opponent)?true:false;
            wall    =(model.total_field[y][x-1]=='|')?true:false; 
            if (x==0) 
              {
                if(player) cout<<"Ходу нема. Зробіть інший вибір";
                //else cout<<"Типа бот не смог сделал ход";
              }
            else if(wall)
              {
                if(player) cout<<"Там стіна. Ходу нема. Зробіть інший вибір";
                //else cout<<"Типа бот не смог сделал ход";
              }
            else if(opponent)
              {
                wall    =(model.total_field[y][x-3]=='|')?true:false;
                if (x>2 && !wall)
                {
                  if(player)
                  {
                    model.total_field[y][x]=' ';
                    model.player1_coordinates[1]-=4;
                    x-=4;
                    model.total_field[y][x]='H';
                    View::draw_field(model);
                    player = !player;
                  }
                  else
                  {
                    model.total_field[y][x]=' ';
                    model.player2_coordinates[1]-=4;
                    x-=4;
                    model.total_field[y][x]='C';
                    View::draw_field(model);
                    player = !player;
                  }
                }
                else if (x>2 && wall)
                {
                  model.total_field[y][x]=' ';

                  int side_wall, top_wall, bottom_wall;
                  top_wall =(model.total_field[y_opponent-1][x_opponent]=='_')?1:0;
                  bottom_wall=(model.total_field[y_opponent+1][x_opponent]=='_')?2:0;
                  side_wall= top_wall + bottom_wall;
                  switch (side_wall)
                  {
                    case 0:
                      if(player)
                      {
                        cout<<"Виберіть, куди ставити свою шашку: вверх (8) чи вниз (2) від шашки опонента?"<<endl;

                        cin >>direction;
                        if     (direction==8) {  model.player1_coordinates[0]-=2; model.player1_coordinates[1]-=2; player = !player;}
                        else if(direction==2) {  model.player1_coordinates[0]+=2; model.player1_coordinates[1]-=2; player = !player;}
                        else  
                        {  
                          cout<<"Ви вибрали неіснуючий напрям, тому шашка встановлено верх."<<endl;
                          model.player1_coordinates[0]-=2; model.player1_coordinates[1]-=2;
                          player = !player;
                        }
                      }
                      else
                      {
                        direction=2+rand()%2*6;
                        if     (direction==8) {  model.player2_coordinates[0]-=2; model.player2_coordinates[1]-=2; player = !player;}
                        else if(direction==2) {  model.player2_coordinates[0]+=2; model.player2_coordinates[1]-=2; player = !player;}                    
                      }
                      break;
                    case 1:
                      if(player)
                      {
                        model.player1_coordinates[0]+=2; model.player1_coordinates[1]-=2;
                        player = !player;
                      }
                      else
                      {
                        model.player2_coordinates[0]+=2; model.player2_coordinates[1]-=2;
                        player = !player;
                      }
                      break;
                    case 2:
                      if(player)
                      {
                        model.player1_coordinates[0]-=2; model.player1_coordinates[1]-=2;
                        player = !player;
                      }
                      else
                      {
                        model.player2_coordinates[0]-=2; model.player2_coordinates[1]-=2;
                        player = !player;
                      }
                      break;
                    case 3:
                      if(player)
                      {
                        cout<<"Перескок неможливий. Виберіть інший хід."<<endl;
                        std::this_thread::sleep_for (std::chrono::seconds(3));    
                      }
                      break;
                    default:
                      break;
                  }
                  if(player)
                  {
                    model.total_field[model.player1_coordinates[0]][model.player1_coordinates[1]]='H';
                    View::draw_field(model);
                  }
                  else 
                  {
                    model.total_field[model.player2_coordinates[0]][model.player2_coordinates[1]]='C';
                    View::draw_field(model);                   
                  }
                }
                else if (x==2)
                {
                  if(player)
                  {
                    model.total_field[y][x]=' ';
                    model.player1_coordinates[0]-=2;
                    model.player1_coordinates[1]-=2;
                    y-=2;
                    x-=2;
                    model.total_field[y][x]='H';
                    View::draw_field(model);
                    player = !player;
                  }
                  else
                  {
                    model.total_field[y][x]=' ';
                    model.player2_coordinates[0]-=2;
                    model.player2_coordinates[1]-=2;
                    y-=2;
                    x-=2;
                    model.total_field[y][x]='C';
                    View::draw_field(model);
                    player = !player;
                  }
                }
              }
            else 
              {
                if(player)
                {
                  model.total_field[y][x]=' ';
                  model.player1_coordinates[1]-=2;
                  x -= 2;
                  model.total_field[y][x]='H';
                  View::draw_field(model);
                  player = !player;
                }
                else
                {
                  model.total_field[y][x]=' ';
                  model.player2_coordinates[1]-=2;
                  x -= 2;
                  model.total_field[y][x]='C';
                  View::draw_field(model);
                  player = !player;
                }
              }
            break;
          case 6:
            opponent=(x_opponent-x==2 && y==y_opponent)?true:false;
            wall    =(model.total_field[y][x+1]=='|')?true:false; 
            if (x==16) 
              {
                if(player) cout<<"Ходу нема. Зробіть інший вибір";
                //else cout<<"Типа бот не смог сделал ход";
              }
            else if(wall)
              {
                if(player) cout<<"Там стіна. Ходу нема. Зробіть інший вибір";
                //else cout<<"Типа бот не смог сделал ход";
              }
            else if(opponent)
              {
                wall    =(model.total_field[y][x+3]=='|')?true:false;
                if (x<14 && !wall)
                {
                  if(player)
                  {
                    model.total_field[y][x]=' ';
                    model.player1_coordinates[1]+=4;
                    x+=4;
                    model.total_field[y][x]='H';
                    View::draw_field(model);
                    player = !player;
                  }
                  else
                  {
                    model.total_field[y][x]=' ';
                    model.player2_coordinates[1]+=4;
                    x+=4;
                    model.total_field[y][x]='C';
                    View::draw_field(model);
                    player = !player;
                  }
                }
                else if (x<14 && wall)
                {
                  model.total_field[y][x]=' ';
                  int side_wall, top_wall, bottom_wall;
                  top_wall =(model.total_field[y_opponent-1][x_opponent]=='_')?1:0;
                  bottom_wall=(model.total_field[y_opponent+1][x_opponent]=='_')?2:0;
                  side_wall= top_wall + bottom_wall;
                  switch (side_wall)
                  {
                    case 0:
                      if(player)
                      {
                        cout<<"Виберіть, куди ставити свою шашку: вверх (8) чи вниз (2) від шашки опонента?"<<endl;

                        cin >>direction;
                        if     (direction==8) {  model.player1_coordinates[0]-=2; model.player1_coordinates[1]+=2; player = !player;}
                        else if(direction==2) {  model.player1_coordinates[0]+=2; model.player1_coordinates[1]+=2; player = !player;}
                        else  
                        {  
                          cout<<"Ви вибрали неіснуючий напрям, тому шашка встановлено верх."<<endl;
                          model.player1_coordinates[0]-=2; model.player1_coordinates[1]+=2;
                          player = !player;
                        }
                      }
                      else
                      {
                        direction=2+rand()%2*6;
                        if     (direction==8) {  model.player2_coordinates[0]-=2; model.player2_coordinates[1]+=2; player = !player;}
                        else if(direction==2) {  model.player2_coordinates[0]+=2; model.player2_coordinates[1]+=2; player = !player;}
                      }
                      break;
                    case 1:
                      if(player)
                      {
                        model.player1_coordinates[0]+=2; model.player1_coordinates[1]+=2;
                        player = !player;
                      }
                      else
                      {
                        model.player2_coordinates[0]+=2; model.player2_coordinates[1]+=2;
                        player = !player;
                      }
                      break;
                    case 2:
                      if(player)
                      {
                        model.player1_coordinates[0]-=2; model.player1_coordinates[1]+=2;
                        player = !player;
                      }
                      else
                      {
                        model.player2_coordinates[0]-=2; model.player2_coordinates[1]+=2;
                        player = !player;
                      }
                      break;
                    case 3:
                      if(player)
                      {
                        cout<<"Перескок неможливий. Виберіть інший хід."<<endl;
                        std::this_thread::sleep_for (std::chrono::seconds(3));
                      }
                      break;
                    default:
                      break;
                  }
                  if(player)
                  {
                    model.total_field[model.player1_coordinates[0]][model.player1_coordinates[1]]='H';
                    View::draw_field(model);
                  }
                  else 
                  {
                    model.total_field[model.player2_coordinates[0]][model.player2_coordinates[1]]='C';
                    View::draw_field(model);
                  }
                }
                else if (x==14)
                {
                  if(player)
                  {
                    model.total_field[y][x]=' ';
                    model.player1_coordinates[0]-=2;
                    model.player1_coordinates[1]+=2;
                    y-=2;
                    x+=2;
                    model.total_field[y][x]='H';
                    View::draw_field(model);
                    player = !player;
                  }
                  else
                  {
                    model.total_field[y][x]=' ';
                    model.player2_coordinates[0]-=2;
                    model.player2_coordinates[1]+=2;
                    y-=2;
                    x+=2;
                    model.total_field[y][x]='C';
                    View::draw_field(model);
                    player = !player;
                  }
                }
              }
            else 
              {
                if(player)
                {
                  model.total_field[y][x]=' ';
                  model.player1_coordinates[1]+=2;
                  x+=2;
                  model.total_field[y][x]='H';
                  View::draw_field(model);
                  player = !player;
                }
                else
                {
                  model.total_field[y][x]=' ';
                  model.player2_coordinates[1]+=2;
                  x+=2;
                  model.total_field[y][x]='C';
                  View::draw_field(model);
                  player = !player;
                }
              }
            break;
          case 8: 
            opponent=(y-y_opponent==2 && x==x_opponent)?true:false;
            wall    =(model.total_field[y-1][x]=='_')?true:false;
            if (y==0) 
              {
                if(player) {}
              }
            else if(wall)
              {
                if(player) cout<<"Там стіна. Ходу нема. Зробіть інший вибір";
                //"Типа бот не смог сделал ход";
              }
            else if(opponent)
              { 
                wall    =(model.total_field[y-3][x]=='_')?true:false;
                if (y>2 && !wall)
                {
                  if(player)
                  {
                    model.total_field[y][x]=' ';
                    model.player1_coordinates[0]-=4;
                    y-=4;
                    model.total_field[y][x]='H';
                    View::draw_field(model);
                    player = !player;
                  }
                  else
                  {
                    model.total_field[y][x]=' ';
                    model.player2_coordinates[0]-=4;
                    y-=4;
                    model.total_field[y][x]='C';
                    View::draw_field(model);
                    player = !player;
                  }
                }
                else if (y>2 && wall)
                {
                  model.total_field[y][x]=' ';
                  int side_wall, left_wall, right_wall;
                  left_wall =(model.total_field[y_opponent][x_opponent-1]=='|')?1:0;
                  right_wall=(model.total_field[y_opponent][x_opponent+1]=='|')?2:0;
                  side_wall= left_wall + right_wall;
                  switch (side_wall)
                  {
                    case 0:
                      if(player)
                      {
                        cout<<"Виберіть, куди ставити свою шашку: ліворуч (4) чи праворуч (6) від шашки опонента?"<<endl;
                        cin >>direction;
                        if     (direction==4) {  model.player1_coordinates[0]-=2; model.player1_coordinates[1]-=2; player = !player;}
                        else if(direction==6) {  model.player1_coordinates[0]-=2; model.player1_coordinates[1]+=2; player = !player;}
                        else  
                        {  
                          cout<<"Ви вибрали неіснуючий напрям, тому шашка встановлено зліва."<<endl;
                          model.player1_coordinates[0]-=2; model.player1_coordinates[1]-=2;
                          player = !player;
                        }                        
                      }
                      else
                        {
                          // бот
                          direction=4+rand()%2*2;
                          if     (direction==4) {  model.player2_coordinates[0]-=2; model.player2_coordinates[1]-=2; player = !player;}
                          else if(direction==6) {  model.player2_coordinates[0]-=2; model.player2_coordinates[1]+=2; player = !player;}
                        }
                      break;
                    case 1:
                      if(player)
                      {
                        model.player1_coordinates[0]-=2; model.player1_coordinates[1]+=2;
                        player = !player;
                      }
                      else
                      {
                        model.player2_coordinates[0]-=2; model.player2_coordinates[1]+=2;
                        player = !player;
                      }
                      break;
                    case 2:
                      if(player)
                      {
                        model.player1_coordinates[0]-=2; model.player1_coordinates[1]-=2;
                        player = !player;
                      }
                      else
                      {
                        model.player2_coordinates[0]-=2; model.player2_coordinates[1]-=2;
                        player = !player;
                      }
                      break;
                    case 3:
                      if(player)
                      {
                        cout<<"Перескок неможливий. Виберіть інший хід."<<endl;
                        std::this_thread::sleep_for (std::chrono::seconds(3));
                      }
                      break;
                    default:
                      break;
                  }
                  if(player)
                  {
                    model.total_field[model.player1_coordinates[0]][model.player1_coordinates[1]]='H';
                    View::draw_field(model);
                  }
                  else
                  {
                    model.total_field[model.player2_coordinates[0]][model.player2_coordinates[1]]='C';
                    View::draw_field(model);
                  }
                }
                else if (y==2)
                {
                  if(player)
                  {
                    model.total_field[y][x]=' ';
                    model.player1_coordinates[0]-=2;
                    model.player1_coordinates[1]-=2;
                    y-=2;
                    x-=2;
                    model.total_field[y][x]='H';
                    View::draw_field(model);
                    player = !player;
                  }
                  else
                  {
                    model.total_field[y][x]=' ';
                    model.player2_coordinates[0]-=2;
                    model.player2_coordinates[1]-=2;
                    y-=2;
                    x-=2;
                    model.total_field[y][x]='c';
                    View::draw_field(model);
                    player = !player;
                  }
                }
              }
            else 
              {
                if(player)
                {
                  model.total_field[y][x]=' ';
                  model.player1_coordinates[0]-=2;
                  y-=2;
                  model.total_field[y][x]='H';
                  View::draw_field(model);
                  player = !player;
                }
                else
                {
                  model.total_field[y][x]=' ';
                  model.player2_coordinates[0]-=2;
                  y-=2;
                  model.total_field[y][x]='C';
                  View::draw_field(model);
                  player = !player;
                }
              }
            break;
          case 5:
            int wall_type, row, col; 
            wall_start:
            if(player && model.player1_fences == 0)
            {
              cout<<"Всі стінки використані ві не можете поставити ще одну!"<<endl;
              break;
            }
            else if(!player && model.player2_fences == 0)
            {
              break;
            }
            if(player)
            {
              cout<<"Виберіть тип стіни: 1 - вертикальна, 0 - горизонтальна"<<endl;
              cin >>wall_type;
            } 
            else
            {
              wall_type=rand()%2;
            }
            if (wall_type==0)
            {
              if(player) cout<<"Задайте координати"<<endl;
              do
              {
                if(player)
                {
                  cout<<"Виберіть ряд від 1 до 8"<<endl;
                  cin >>row;
                  cout<<"Виберіть координату від 1 до 9 зліва направо"<<endl;
                  cin >>col;
                }
                else
                {
                  row=1+rand()%8;
                  col=1+rand()%8;
                }
                wall_is_present=model.total_field[row*2-1][col*2-2]=='+' || model.total_field[row*2-1][col*2-1]=='+' || model.total_field[row*2-1][col*2-0]=='+' ||model.total_field[row*2-1][col*2-2]=='+' || model.total_field[row*2-1][col*2-1]=='+' || model.total_field[row*2-1][col*2-0]=='+';
                if(wall_is_present && player) cout<<"Вказане місце для стіни зайняте. Задайте координати знову."<<endl;
              }while(wall_is_present);

              model.total_field[row*2-1][col*2-2]='+';
              model.total_field[row*2-1][col*2-1]='+';
              model.total_field[row*2-1][col*2+0]='+';
              WayControl simulation(model);
              if (simulation.possible_way(simulation)) 
              {
                if(player) model.player1_fences --;
                else model.player2_fences --;
                View::draw_field(model);
                player = !player;
              }
              else 
                {
                  model.total_field[row*2-1][col*2-2]=' ';
                  model.total_field[row*2-1][col*2-1]=' ';
                  model.total_field[row*2-1][col*2+0]=' ';
                  if(player) View::no_way();
                  goto wall_start;
                }
            }
            else if (wall_type==1)
            {
              if(player) cout<<"Задайте координати"<<endl;
              do
              {
                if(player)
                {
                  cout<<"Виберіть колонку від 1 до 8"<<endl;
                  cin >>col;
                  cout<<"Виберіть координату від 1 до 9 зверху вниз"<<endl;
                  cin >>row;
                }
                else
                {
                  row=1+rand()%8;
                  col=1+rand()%8;
                }
                wall_is_present=model.total_field[row*2-2][col*2-1]=='+' || model.total_field[row*2-1][col*2-1]=='+' || model.total_field[row*2-0][col*2-1]=='+'||model.total_field[row*2-2][col*2-1]=='+' || model.total_field[row*2-1][col*2-1]=='+' || model.total_field[row*2-0][col*2-1]=='+';
                if(wall_is_present && player) 
                  { cout<<"Вказане місце для стіни зайняте. Задайте координати знову."<<endl;}
              }while(wall_is_present);
              model.total_field[row*2-2][col*2-1]='+';
              model.total_field[row*2-1][col*2-1]='+';
              model.total_field[row*2-0][col*2-1]='+';
              WayControl simulation(model);
              if (simulation.possible_way(simulation))
              {
                if(player) model.player1_fences --;
                else model.player2_fences --;
                View::draw_field(model);
                player = !player;
              }
              else 
                {
                  model.total_field[row*2-2][col*2-1]=' ';
                  model.total_field[row*2-1][col*2-1]=' ';
                  model.total_field[row*2-0][col*2-1]=' ';
                  if(player) View::no_way();
                  goto wall_start;
                }
            }
            else {if(player) cout<<"Натиснули недозволену клавішу. Зробіть інший вибір"<<endl;}
            break;
          default:
                if(player) cout<<"Натиснули недозволену клавішу. Зробіть інший вибір"<<endl;
                break;
        }
  }
  static void move(bool &player, Model &model)
  {
    srand(time(0));
    if(player)
      {
        int x, y, x_opponent, y_opponent, choice;
        y=model.player1_coordinates[0];
        x=model.player1_coordinates[1];
        x_opponent=model.player2_coordinates[1];
        y_opponent=model.player2_coordinates[0];
        cout<<"Для здійснення ходу використовуйте нампад:\n8 - вверх, 2 - вниз, 4 - вліво, 6 - вправо, 5 - встановити стіну"<<endl;
        cin >>choice;
        choice_revision(choice,player,model,x,y,x_opponent,y_opponent);
        if(model.player1_coordinates[0]==0) win = "людина.";
      }
    else //хід комп'ютера
      {
        int x, y, x_opponent, y_opponent, choice;
        int choices[]={2,4,5,6,8};
        choice=choices[rand()%5];
        y=model.player2_coordinates[0];
        x=model.player2_coordinates[1];
        x_opponent=model.player1_coordinates[1];
        y_opponent=model.player1_coordinates[0];
        choice_revision(choice,player,model,x,y,x_opponent,y_opponent);
        if(model.player2_coordinates[0]==16) win = "комп'ютер.";
      }
  }

};
int main() {
  std::cout << "          Quoridor1!\n";
  char c;
  one:
  win="";
  Model model;
  View::draw_field(model);
  do
    {
      Control::move(model.player,model);
    }while (win=="");

  cout<<"Переможцем стає - "<<win<<endl;
  cout<<"Для перезапуску натисніть r"<<endl;
  cin>>c;

  if(c=='r' || c=='R')
  goto one;

} 