//
// Created by HarryXiong on 2023/5/15.
//

#include <cstring>
#include <string>
#include "CTmRadar.h"
#include <format>
#include <atltypes.h>
#define PI acos(-1)

using namespace std;

namespace EuroScopePlugIn {
    bool CTmRadar::OnCompileCommand(const char *sCommandLine) {
        char *cmd = const_cast<char *>(sCommandLine);
        char* cmdHead = strtok(cmd," ");
        if(!stricmp(cmdHead,".dw")){
            dolog("Start to Draw");
            drawing=true;
            startPos = endPos = CPosition();
            size = angle = 0;
            return true;
        }
        return false;
    }

    void CTmRadar::OnAsrContentToBeClosed() {

    }

    void CTmRadar::dolog(const char * msg) {
        this->GetPlugIn()->DisplayUserMessage(this->GetPlugIn()->GetPlugInName(),this->GetPlugIn()->GetPlugInName(),msg,
                                              false, false, false, false, false);
    }

    CTmRadar::CTmRadar() {
//        AddScreenObject(0,"Hello",this->GetRadarArea(),false,"Hello World");
    }

    void CTmRadar::OnButtonDownScreenObject(int ObjectType, const char *sObjectId, POINT Pt, RECT Area, int Button) {
        if(!drawing)return;
        if(startPos.m_Latitude - 0<0.00005){
            startPos = CPosition(ConvertCoordFromPixelToPosition(Pt));
            dolog(format("start position is {},{}",startPos.m_Latitude,startPos.m_Longitude).c_str());
        }else if(endPos.m_Latitude - 0<0.00005){
            endPos = CPosition(ConvertCoordFromPixelToPosition(Pt));
            POINT startPosPix = ConvertCoordFromPositionToPixel(startPos);
            POINT endPosPix = ConvertCoordFromPositionToPixel(endPos);
            float rawAngle = atan((float)(endPosPix.y-startPosPix.y)/(float)(endPosPix.x-startPosPix.x))*180.0/PI;
            if(rawAngle<0){
                rawAngle = endPosPix.y>startPosPix.y?rawAngle+180:rawAngle+360;
            }else{
                rawAngle = endPosPix.y>startPosPix.y?rawAngle:rawAngle+180;
            }
            angle = rawAngle;
            dolog(format("end position is {},{}",endPos.m_Latitude,endPos.m_Longitude).c_str());
            dolog(format("The angle is {}",rawAngle).c_str());
        }else{
            POINT startPosPix = ConvertCoordFromPositionToPixel(startPos);
            POINT endPosPix = ConvertCoordFromPositionToPixel(endPos);
            POINT mousePix;
            POINT startSidePix1;
            POINT startSidePix2;
            POINT endSidePix1;
            POINT endSidePix2;
            POINT endOutPix1;
            POINT endOutPix2;
            POINT supEndPix;
            GetCursorPos(&mousePix);
            size = abs(((endPosPix.y - mousePix.y) * cos(angle*PI/180)) - ((endPosPix.x - mousePix.x) * sin(angle*PI/180)));
            double dirX = sin(angle*PI/180);
            double dirY = cos(angle*PI/180);
            startSidePix1.x = startPosPix.x + size*dirX;
            startSidePix1.y = startPosPix.y - size*dirY;
            startSidePix2.x = startPosPix.x - size*dirX;
            startSidePix2.y = startPosPix.y + size*dirY;
            double supX = cos(angle*PI/180);
            double supY = sin(angle*PI/180);
            supEndPix.x = endPosPix.x - 2*size*supX;
            supEndPix.y = endPosPix.y - 2*size*supY;
            endSidePix1.x = supEndPix.x + size*dirX;
            endSidePix1.y = supEndPix.y - size*dirY;
            endSidePix2.x = supEndPix.x - size*dirX;
            endSidePix2.y = supEndPix.y + size*dirY;
            endOutPix1.x = supEndPix.x + 2*size*dirX;
            endOutPix1.y = supEndPix.y - 2*size*dirY;
            endOutPix2.x = supEndPix.x - 2*size*dirX;
            endOutPix2.y = supEndPix.y + 2*size*dirY;
            startSidePos1= ConvertCoordFromPixelToPosition(startSidePix1);
            startSidePos2 = ConvertCoordFromPixelToPosition(startSidePix2);
            endSidePos1 = ConvertCoordFromPixelToPosition(endSidePix1);
            endSidePos2 = ConvertCoordFromPixelToPosition(endSidePix2);
            endOutPos1=ConvertCoordFromPixelToPosition(endOutPix1);
            endOutPos2=ConvertCoordFromPixelToPosition(endOutPix2);
            drawing= false;
        }
    }

    void CTmRadar::OnRefresh(HDC hDC, int Phase) {
        if(Phase == REFRESH_PHASE_AFTER_TAGS){
            if(drawing) AddScreenObject(0,"Drawing",this->GetRadarArea(),false,"Hello World");
            if(startPos.m_Latitude - 0>0.00005&&endPos.m_Latitude - 0<0.00005){
                POINT startPosPix = ConvertCoordFromPositionToPixel(startPos);
                POINT endPosPix;
                GetCursorPos(&endPosPix);
                MoveToEx(hDC,startPosPix.x,startPosPix.y,NULL);
                LineTo(hDC,endPosPix.x,endPosPix.y);
                int argu = 30;
                float angle = atan((float)(endPosPix.y-startPosPix.y)/(float)(endPosPix.x-startPosPix.x))*180.0/PI;
                if(angle<0){
                    angle = endPosPix.y>startPosPix.y?angle+180:angle+360;
                }else{
                    angle = endPosPix.y>startPosPix.y?angle:angle+180;
                }
                LineTo(hDC,endPosPix.x-argu*cos((angle+45)*PI/180),endPosPix.y-argu*sin((angle+45)*PI/180));
                MoveToEx(hDC,endPosPix.x,endPosPix.y,NULL);
                LineTo(hDC,endPosPix.x-argu*cos((angle-45)*PI/180),endPosPix.y-argu*sin((angle-45)*PI/180));

            }else if(startPos.m_Latitude - 0>0.00005&&endPos.m_Latitude - 0>0.00005){
                POINT startPosPix = ConvertCoordFromPositionToPixel(startPos);
                POINT endPosPix = ConvertCoordFromPositionToPixel(endPos);
                POINT mousePix;
                POINT startSidePix1;
                POINT startSidePix2;
                POINT endSidePix1;
                POINT endSidePix2;
                POINT endOutPix1;
                POINT endOutPix2;
                POINT supEndPix;
                GetCursorPos(&mousePix);
                double argu;
                if(drawing){
                    argu = abs(((endPosPix.y - mousePix.y) * cos(angle*PI/180)) - ((endPosPix.x - mousePix.x) * sin(angle*PI/180)));
                    double dirX = sin(angle*PI/180);
                    double dirY = cos(angle*PI/180);
                    startSidePix1.x = startPosPix.x + argu*dirX;
                    startSidePix1.y = startPosPix.y - argu*dirY;
                    startSidePix2.x = startPosPix.x - argu*dirX;
                    startSidePix2.y = startPosPix.y + argu*dirY;
                    double supX = cos(angle*PI/180);
                    double supY = sin(angle*PI/180);
                    supEndPix.x = endPosPix.x - 2*argu*supX;
                    supEndPix.y = endPosPix.y - 2*argu*supY;
                    endSidePix1.x = supEndPix.x + argu*dirX;
                    endSidePix1.y = supEndPix.y - argu*dirY;
                    endSidePix2.x = supEndPix.x - argu*dirX;
                    endSidePix2.y = supEndPix.y + argu*dirY;
                    endOutPix1.x = supEndPix.x + 2*argu*dirX;
                    endOutPix1.y = supEndPix.y - 2*argu*dirY;
                    endOutPix2.x = supEndPix.x - 2*argu*dirX;
                    endOutPix2.y = supEndPix.y + 2*argu*dirY;
                } else{
                    startSidePix1 = ConvertCoordFromPositionToPixel(startSidePos1);
                    startSidePix2 = ConvertCoordFromPositionToPixel(startSidePos2);
                    endSidePix1 = ConvertCoordFromPositionToPixel(endSidePos1);
                    endSidePix2 = ConvertCoordFromPositionToPixel(endSidePos2);
                    endOutPix1 = ConvertCoordFromPositionToPixel(endOutPos1);
                    endOutPix2 = ConvertCoordFromPositionToPixel(endOutPos2);
                }
                
                MoveToEx(hDC,startPosPix.x,startPosPix.y,NULL);
                LineTo(hDC,startSidePix1.x,startSidePix1.y);
                LineTo(hDC,endSidePix1.x,endSidePix1.y);
                LineTo(hDC,endOutPix1.x,endOutPix1.y);
                LineTo(hDC,endPosPix.x,endPosPix.y);
                LineTo(hDC,endOutPix2.x,endOutPix2.y);
                LineTo(hDC,endSidePix2.x,endSidePix2.y);
                LineTo(hDC,startSidePix2.x,startSidePix2.y);
                LineTo(hDC,startPosPix.x,startPosPix.y);
            }

        }
    }

    void CTmRadar::OnOverScreenObject(int ObjectType, const char *sObjectId, POINT Pt, RECT Area) {
        if(drawing&&startPos.m_Latitude - 0>0.00005){
            RequestRefresh();
        }
    }
} // EuroScopePlugIn