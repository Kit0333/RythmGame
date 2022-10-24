#include <iostream>
#include <headers/game.hpp>
#include <thread>
#include <chrono>

int main(int argc,char* argv[]){
    int userScreenWidth=0;
    int userScreenHeight=0;
    Game game("Chess Game C++",SDL_WINDOW_SHOWN,userScreenWidth,userScreenHeight);

    SDL_Texture* boardImage=game.loadTexture("res/img/board.png");
    SDL_Texture *pawnImage=game.loadTexture("res/img/pawn.png");
    
    /*
    SLD_Texture* bishopImage=game.loadTexture("res/img/bishop.jpg");
    SLD_Texture* knightImage=game.loadTexture("res/img/knight.jpg");
    SDL_Texture* rookImage=game.loard("res/img/rook.jpg");
    SLD_Texture* queenImage=game.loadTexture("res/img/queen.jpg");
    SLD_Texture* kingImage=game.loadTexture("res/img/king.jpg");
    */


    int posXMouse(0);
    int posYMouse(0);
    Uint32 click;
    Board *board=new Board();
    int actualBoardNum(-1);
    int selectedPawn[2]={-1,-1};
    int xPawn(0);
    int yPawn(0);
    bool hasBeenChosed=false;
    bool canAttack;
    int hasBeenChosedNumber(-1);

    

    while (game.getGameState()!=GameState::EXIT){
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {

        case SDL_QUIT://Shut down the game
            game.gameStateChange(GameState::EXIT);
            break;

        case SDL_MOUSEBUTTONDOWN://Get mouse position
            int mouseButton=game.mousePress(event.button);
            if (mouseButton==0){
                SDL_GetMouseState(&posXMouse,&posYMouse);
                if (posXMouse>840){
                    xPawn=(posXMouse-840)%135;
                    yPawn=posYMouse%135;
                    if(game.getActualPlayer()==Player::J1){
                        //We check which case got clicked on which turn (you can't click on ennemy's pawn on your turn )
                        //So later we can show on which case you can go with the specific pawn
                        if(board->getBoardN(xPawn,yPawn)<7 && board->getBoardN(xPawn,yPawn)!=0 && hasBeenChosed==false){
                            /*When nodoy is ready to attack, in selectedPawn we stock x,y of the pawn that we selected to be ready to attack*/
                            selectedPawn[0]=xPawn;
                            selectedPawn[1]=yPawn;
                        }
                        else if (board->getBoardN(xPawn,yPawn)>10 || board->getBoardN(xPawn,yPawn)&& hasBeenChosed==true ){// we verify if the selected one where we attack is an ennemy or a void case 
                            //One pawn is already selected to attack, now it's the click to attack
                            canAttack==true;
                            hasBeenChosed=false;
                            board->modify(0,selectedPawn[0],selectedPawn[1]);
                            board->modify(hasBeenChosedNumber,xPawn,yPawn);
                            hasBeenChosed=false;//reset after attack
                            hasBeenChosedNumber=-1;//reset after attack
                        }
                    }
                    else{
                        if(board->getBoardN(xPawn,yPawn)>6 && hasBeenChosed==false){
                            selectedPawn[0]=xPawn;
                            selectedPawn[1]=yPawn;
                            }
                        else if (board->getBoardN(xPawn,yPawn)<7 && hasBeenChosed==true){
                            canAttack==true;
                            board->modify(0,selectedPawn[0],selectedPawn[1]);
                            board->modify(hasBeenChosedNumber,xPawn,yPawn);
                            hasBeenChosed=false;
                            hasBeenChosedNumber=-1;
                        }
                        }
                }
            break;
            }
        //starting rendering
        game.clear();
        //renders the board
        game.render(boardImage,0,0,userScreenWidth,userScreenHeight);
        //renders the pawn on the board
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                actualBoardNum=board->getBoardN(i,j);
                //if(hasBeenChosed==true){
                //    int chosenNumber=board->getBoardN(onClickPawn[0],onClickPawn[1]);
                //}
                switch(actualBoardNum){
                    //The side on the bot (white)
                    case 1 :
                        if (board->getBoardN(xPawn,yPawn)==1 &&yPawn==i && xPawn==j && hasBeenChosed==false){
                            game.render(pawnImageChosen,845+(135*j),i*135+5,127,127);
                            hasBeenChosed=true;
                            hasBeenChosedNumber=board->getBoardN(i,j);
                        }
                        else{            
                            game.render(pawnImage,845+(135*j),i*135+5,127,127);
                        }
                        break;
                    case 2:
                        if (board->getBoardN(xPawn,yPawn)==2 &&yPawn==i && xPawn==j && hasBeenChosed==false){
                            game.render(bishopImageChosen,845+(135*j),i*135+5,127,127);
                            hasBeenChosed=true;
                            hasBeenChosedNumber=board->getBoardN(i,j);
                        }
                        else{
                            game.render(bishopImage,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 3:
                        if (board->getBoardN(xPawn,yPawn)==3 &&yPawn==i && xPawn==j && hasBeenChosed==false){
                                game.render(knightImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                                hasBeenChosedNumber=board->getBoardN(i,j);
                        }
                        else{
                            game.render(knightImage,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 4:
                        if (board->getBoardN(xPawn,yPawn)==4 &&yPawn==i && xPawn==j && hasBeenChosed==false){
                                game.render(rookImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                                hasBeenChosedNumber=board->getBoardN(i,j);
                        }
                        else{
                            game.render(rookImage,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 5:
                        if (board->getBoardN(xPawn,yPawn)==5 &&yPawn==i && xPawn==j && hasBeenChosed==false){
                                game.render(queenImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                                hasBeenChosedNumber=board->getBoardN(i,j);
                        }
                        else{
                            game.render(queenImage,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 6:
                        if (board->getBoardN(xPawn,yPawn)==6 &&yPawn==i,&& xPawn==j){
                                game.render(kingImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                                hasBeenChosedNumber=board->getBoardN(i,j);
                        }
                        else{
                            game.render(kingImage,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 11 :                        
                        if (board->getBoardN(xPawn,yPawn)==11 &&yPawn==i,&& xPawn==j && hasBeenChosed==false){
                                game.render(pawn2ImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                                hasBeenChosedNumber=board->getBoardN(i,j);
                        }
                        else{
                            if(hasBeenChosed==true && game.getActualPlayer()==Player::J1 ){//we check if this pawn can be attacked
                                switch(board->canBeAttacked(hasBeenChosedNumber,j,i,xPawn,yPawn)){
                                    case 0://it return 0 when this
                                        break;
                                        //we check if he can  touch him
                                        //(board->getBoardN(onClickPawn[0]+1,onClickPawn[1]-1)==board->getBoardN(i,j) || board->getBoardN(onClickPawn[0]+1,onClickPawn[1]+1)==board->getBoardN(i,j))
                                        //the code above is a test ,currently I'm not sure about the verification
                                    default:    
                                        game.render(pawn2ImageCanBeenAttacked,845+(135*j),i*135,130,130);
                                        break;
                                    
                                        
                                }
                                
                            }
                            else{//normal render when this pawn is not the selected one and it can be attacked by ennemy's pawn
                            game.render(pawn2Image,845+(135*j),i*135,130,130);
                            }
                        }
                        break;
                    case 12:
                        if (board->getBoardN(xPawn,yPawn)==12 &&yPawn==i && xPawn==j){
                                game.render(bishop2ImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                        }
                        else{
                            game.render(bishop2Image,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 13:
                        if (board->getBoardN(xPawn,yPawn)==13 &&yPawn==i && xPawn==j){
                                game.render(knight2ImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                        }
                        else{
                            game.render(knight2Image,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 14:
                        if (board->getBoardN(xPawn,yPawn)==14 &&yPawn==i && xPawn==j){
                                game.render(rook2ImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                        }
                        else{
                            game.render(rook2Image,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 15:
                        if (board->getBoardN(xPawn,yPawn)==15 &&yPawn==i && xPawn==j){
                                game.render(quueen2ImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                        }
                        else{
                            game.render(queen2Image,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 16:
                        if (board->getBoardN(xPawn,yPawn)==3 &&yPawn==i && xPawn==j){
                                game.render(king2ImageChosen,845+(135*j),i*135+5,127,127);
                                hasBeenChosed=true;
                        }
                        else{
                            game.render(king2Image,845+(135*j),i*135,130,130);
                        }
                        break;
                    case 0://if there is no pawn on this case, i will do it at the end
                }
            }
        }
        game.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    return 0;
}

