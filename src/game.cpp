#include <headers/game.hpp>
#include <iostream>
GameState Game::getGameState(){
    return m_gameState;
};

void Game::gameStateChange(GameState x){
    m_gameState=x;
};

Player Game::getActualPlayer(){
    return m_player;
};

void Game::playerSwitch(){
    if (m_player==Player::J1){
        m_player=Player::J2;
    }
    else{
        m_player=Player::J1;
    }
}

Game::Game(const char* title,Uint32 flags,int &width,int &height) {
    SDL_Init(SDL_INIT_EVERYTHING);
    
    UserScreenSize(width,height);
    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, width,height, flags);
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    m_gameState = GameState::PLAY;
    m_player=Player::J1;
};

Game::~Game() {};

void Game::UserScreenSize(int &width,int &height){
    //fonction retournant rien 
    //la fonction utilise 2 références , pour la longueur et la hauteur de l'écran

    SDL_DisplayMode displaymode;
    if(SDL_GetCurrentDisplayMode(0,&displaymode)!=0){
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        width=1024;
        height=600;
    }
    else{
        width=displaymode.w;
        height=displaymode.h;
    }

};

void Game::run(){
    clear();
    display();
}

void Game::render(SDL_Texture* tex,int x,int y,int w,int h){
    /*SDL_Rect src;
    src.x=x;
    src.y=y;
    src.w=w;
    src.h=h;
*/
    SDL_Rect dist;
    dist.x=x;
    dist.y=y;
    dist.w=w;
    dist.h=h;

    SDL_RenderCopy(m_renderer,tex,NULL,&dist);
};


SDL_Texture* Game::loadTexture(const char* filePath){
    
    SDL_Texture* tex=NULL;
    tex=IMG_LoadTexture(m_renderer,filePath);

    if(tex==NULL){
        std::cout<<"img couldn't load from file path "<<filePath<<" ERROR : "<<SDL_GetError()<<std::endl;
    }
    return tex;

};

void Game::display(){
    SDL_RenderPresent(m_renderer);

}

void Game::clear(){
    SDL_RenderClear(m_renderer);
}

void Game::cleanup(){
    SDL_DestroyWindow(m_window);
}


int& Game::mousePress(SDL_MouseButtonEvent& b){
    static int state;
    if(b.button==SDL_BUTTON_LEFT){
        state=0;//0= left click
    }
    else{
        state=1;//1=right click
    }
    return state;
};


Board::Board(){
    static int  boardGame[8][8]={                // Index:
                        {14,13,12,15,16,12,13,14},//  0-7
                        {11,11,11,11,11,11,11,11},//  8-15
                        {0,0,0,0,0,0,0,0},//  16-23
                        {0,0,0,0,0,0,0,0},//  24-31
                        {0,0,0,0,0,0,0,0},//  32-39
                        {0,0,0,0,0,0,0,0},//  40-47
                        {1,1,1,1,1,1,1,1},//  48-55
                        {4,3,2,6,5,2,3,4}};// 56-63
    m_board=&boardGame[0][0];//Pointer to the first element
    Pawn *m_pawn=new Pawn(1);
    Pawn *m_bishop=new Pawn(2);
    Pawn *m_knight=new Pawn(3);
    Pawn *m_rook=new Pawn(4);
    Pawn *m_queen=new Pawn(5);
    Pawn *m_king=new Pawn(6);

};
Board::~Board() {
    delete m_pawn;
    delete m_bishop;
    delete m_knight;
    delete m_rook;
    delete m_queen;
    delete m_king;
};

int Board::getBoardN(int y,int x){
    return *(m_board+(y*8)+x);
};
Pawn::Pawn(int type){
    m_type=type;
    switch(type){
            case 1:
                m_info="res/img/pawnHelp";
                break;
            case 2:
                m_info="res/img/bishopHelp";
                break;
            case 3:
                m_info="res/img/knightHelp";
                break;
            case 4:
                m_info="res/img/rookHelp";
                break;
            case 5:
                m_info="res/img/queenHelp";
                break;
            case 6:
                m_info="res/img/kingHelp";
                break;
            }

    

    /*
    ----Unit type information : 
    1: Pawn
    2: Bishop
    3: The knight
    4: Rook
    5: Queen
    6: King  
    
    */
};

int Board::trace(int &x1,int&y1,int&x2,int&y2){
    return ((y1*8)+x1)-((y2*8)+x2);
};
void Board::modify(int newNum,int x,int y){
    *(m_board+(y*8)+x)=newNum;
};
int Board::canBeAttacked(int ennemyPawnType,int ennemyX,int ennemyY ,int thisX,int thisY,int thisPawnType,Game game){
    int count(1);
    int playerMove;
    if(game.getActualPlayer()==Player::J1){
        playerMove=0;
    }
    else{
        playerMove=10;
    }
    switch(ennemyPawnType){
        case 11:
            ennemyPawnType=1;
            break;
        case 12:
            ennemyPawnType=2;
            break; 
        case 13:
            ennemyPawnType=3;
            break;
        case 14:
            ennemyPawnType=4;
            break;
        case 15:
            ennemyPawnType=5;
            break;
        case 16:
            ennemyPawnType=6;
            break;
    }

    switch(ennemyPawnType){
        case 1://if ennemy is a white pawn
            if ((this->trace(ennemyX,ennemyY,thisX,thisY)==9 &&thisPawnType>10 && thisPawnType!=0)||(this->trace(ennemyX,ennemyY,thisX,thisY)==7 &&thisPawnType>10 &&thisPawnType!=0)||(this->trace(ennemyX,ennemyY,thisX,thisY)==8 && thisPawnType==0)){
                return 1+playerMove;
            }
            else{
                return 0;
            }
            break;
        case 2://if ennemy is a white bishop
            count=1;
            if (thisPawnType<10 &&thisPawnType!=0){
                return 0;
            }
            if ((thisY<ennemyY) &&(thisX>ennemyX)){
                for(int i=1;i<-(thisY-ennemyY);i++){
                    if(this->getBoardN(ennemyY-i,ennemyX+i)==0){
                        count++;
                    }
                }
                if ((count)==-(thisY-ennemyY)&&count==(thisX-ennemyX)){
                    return 2+playerMove;
                }
                else{
                    return 0;
                }
            }
            else if ((thisY<ennemyY) &&(thisX<ennemyX)){
                for(int i=1;i<-(thisY-ennemyY);i++){
                    if(this->getBoardN(ennemyY-i,ennemyX-i)==0){
                        count++;
                    }
                    
                }
                if((count)==(-(thisY-ennemyY))&&count==(-(thisX-ennemyX))){
                    return 2+playerMove;
                }
                else{
                    return 0;
                }
            }
            else if ((thisY>ennemyY) &&(thisX>ennemyX)){
                for(int i=1;i<(thisY-ennemyY);i++){
                    if(this->getBoardN(ennemyY+i,ennemyX+i)==0){
                        count++;
                    }
                    
                }
                if(count==(thisY-ennemyY)&&count==(thisX-ennemyX)){
                    return 2+playerMove;
                }
                else{
                    return 0;
                }
            }
            else if ((thisY>ennemyY) &&(thisX<ennemyX)){
                for(int i=1;i<(thisY-ennemyY);i++){
                    if(this->getBoardN(ennemyY+i,ennemyX-i)==0){
                        count++;
                    }
                    
                }
                if (count==(thisY-ennemyY)&&count==(-(thisX-ennemyX))){
                    return 2+playerMove;
                }
                else{
                    return 0;
                }
            }
            break;
        case 3://if ennemy is a white knight
            if (thisPawnType!=11&&thisPawnType!=12&&thisPawnType!=13&&thisPawnType!=14&&thisPawnType!=15&&thisPawnType!=16&&thisPawnType!=0){
                return 0;
            }
            if(((ennemyX-1)==thisX &&(ennemyY-2)==thisY) || ((ennemyX+1)==thisX&&(ennemyY-2)==thisY) ||
               ((ennemyX-2)==thisX &&(ennemyY-1)==thisY) || ((ennemyX+2)==thisX&&(ennemyY-1)==thisY) ||
               ((ennemyX-2)==thisX &&(ennemyY+1)==thisY) || ((ennemyX+2)==thisX&&(ennemyY+1)==thisY) ||
               ((ennemyX-1)==thisX &&(ennemyY+2)==thisY) || ((ennemyX+1)==thisX&&(ennemyY+2)==thisY)  ){
                return 3+playerMove;
               }
            else{
                return 0;
            }
            break;
            
        case 4://if ennemy is a white rook
            if(thisPawnType<10&&thisPawnType!=0){
                return 0;
            }
            count=1;
            if ((ennemyX==thisX)&&(ennemyY>thisY)){
                for(int i=1;i<(ennemyY-thisY);i++){
                    if(this->getBoardN(thisY+i,thisX)==0){
                        count++;
                    }
                }
                if(count==(ennemyY-thisY)){
                    return 4+playerMove;
                }
                else{
                    return 0;
                }
            }
            else if ((ennemyX==thisX)&&(ennemyY<thisY)){
                for(int i=-1;i>(ennemyY-thisY);i--){
                    if(this->getBoardN(thisY+i,thisX)==0){
                        count++;
                    }
                }
                if(count==-(ennemyY-thisY)){
                    return 4+playerMove;
                }
                else{
                    return 0;
                }

            }
            else if ((ennemyY==thisY)&&(ennemyX>thisX)){
                for(int j=1;j<(ennemyX-thisX);j++){
                    if(this->getBoardN(thisY,thisX+j)==0){
                        count++;
                    }
                }
                if(count==(ennemyX-thisX)){
                    return 4+playerMove;
                }
                else{
                    return 0;
                }

            }
            else if ((ennemyY==thisY)&&(ennemyX<thisX)){
                for(int j=1;j<-(ennemyX-thisX);j++){
                    if(this->getBoardN(thisY,thisX-j)==0){
                        count++;
                    }
                }
                if(count==-(ennemyX-thisX)){
                    return 4+playerMove;
                }
                else{
                    return 0;
                }
            }
            break;
        case 5:// if ennemy is a white queen ( can use the code from case 2 and 4( bishop + rook))
            count=1;
            if ((thisY<ennemyY) &&(thisX>ennemyX)){
                for(int i=1;i<-(thisY-ennemyY);i++){
                    if(this->getBoardN(ennemyY-i,ennemyX+i)==0){
                        count++;
                    }
                }
                if ((count)==-(thisY-ennemyY)&&count==(thisX-ennemyX)){
                    return 5+playerMove;
                }
                else{
                    return 0;
                }
            }
            else if ((thisY<ennemyY) &&(thisX<ennemyX)){
                for(int i=1;i<-(thisY-ennemyY);i++){
                    if(this->getBoardN(ennemyY-i,ennemyX-i)==0){
                        count++;
                    }
                    
                }
                if((count)==(-(thisY-ennemyY))&&count==(-(thisX-ennemyX))){
                    return 5+playerMove;
                }
                else{
                    return 0;
                }
            }
            else if ((thisY>ennemyY) &&(thisX>ennemyX)){
                for(int i=1;i<(thisY-ennemyY);i++){
                    if(this->getBoardN(ennemyY+i,ennemyX+i)==0){
                        count++;
                    }
                    
                }
                if(count==(thisY-ennemyY)&&count==(thisX-ennemyX)){
                    return 5+playerMove;
                }
                else{
                    return 0;
                }
            }
            else if ((thisY>ennemyY) &&(thisX<ennemyX)){
                for(int i=1;i<(thisY-ennemyY);i++){
                    if(this->getBoardN(ennemyY+i,ennemyX-i)==0){
                        count++;
                    }
                    
                }
                if (count==(thisY-ennemyY)&&count==(-(thisX-ennemyX))){
                    return 5+playerMove;
                }
                else{
                    return 0;
                }
            }
            else if ((ennemyX==thisX)&&(ennemyY>thisY)){
                for(int i=1;i<(ennemyY-thisY);i++){
                    if(this->getBoardN(thisY+i,thisX)==0){
                        count++;
                    }
                }
                if(count==(ennemyY-thisY)){
                    return 5+playerMove;
                }
                else{
                    return 0;
                }
            }
            else if ((ennemyX==thisX)&&(ennemyY<thisY)){
                for(int i=-1;i>(ennemyY-thisY);i--){
                    if(this->getBoardN(thisY+i,thisX)==0){
                        count++;
                    }
                }
                if(count==-(ennemyY-thisY)){
                    return 5+playerMove;
                }
                else{
                    return 0;
                }

            }
            else if ((ennemyY==thisY)&&(ennemyX>thisX)){
                for(int j=1;j<(ennemyX-thisX);j++){
                    if(this->getBoardN(thisY,thisX+j)==0){
                        count++;
                    }
                }
                if(count==(ennemyX-thisX)){
                    return 5+playerMove;
                }
                else{
                    return 0;
                }

            }
            else if ((ennemyY==thisY)&&(ennemyX<thisX)){
                for(int j=1;j<-(ennemyX-thisX);j++){
                    if(this->getBoardN(thisY,thisX-j)==0){
                        count++;
                    }
                }
                if(count==-(ennemyX-thisX)){
                    return 5+playerMove;
                }
                else{
                    return 0;
                }
            }
            break;
                    
        case 6://if ennemy is the white king
            if (thisPawnType<10&&thisPawnType!=0){
                return 0;
            }
            if (this->trace(ennemyX,ennemyY,thisX,thisY)==9 ||this->trace(ennemyX,ennemyY,thisX,thisY)==8||this->trace(ennemyX,ennemyY,thisX,thisY)==7||this->trace(ennemyX,ennemyY,thisX,thisY)==-7||this->trace(ennemyX,ennemyY,thisX,thisY)==-8 ||this->trace(ennemyX,ennemyY,thisX,thisY)==-9 ||this->trace(ennemyX,ennemyY,thisX,thisY)==-1||this->trace(ennemyX,ennemyY,thisX,thisY)==1){
                return 6+playerMove;
            }
            else{
                return 0;
            }
            break;
        
        }

        
    return 0;    
};

Pawn::~Pawn(){};
