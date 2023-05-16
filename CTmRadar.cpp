//
// Created by HarryXiong on 2023/5/15.
//

#include <cstring>
#include <string>
#include "CTmRadar.h"
#include <format>
#include <atltypes.h>
#include <fstream>

#define PI acos(-1)

using namespace std;

namespace EuroScopePlugIn {
    bool CTmRadar::OnCompileCommand(const char *sCommandLine) {
        char *cmd = const_cast<char *>(sCommandLine);
        char *cmdHead = strtok(cmd, " ");
        if (!stricmp(cmdHead, ".dw")) {
            dolog("Start to Draw");
            drawing = 1;
            currentArrow = new Arrow();
            return true;
        }
        if (!stricmp(cmdHead, ".popdw")) {
            dolog("Poped one out of the back of the list");
            arrows.pop_back();
            return true;
        }
        if (!stricmp(cmdHead, ".clrdw")) {
            dolog("Cleared all of the dw from the list");
            arrows.clear();
            return true;
        }
        if (!stricmp(cmdHead, ".expdw")) {
            char *p = strtok(NULL, " ");
            if (p == nullptr) {
                p = (char *) "C:/tsmap.txt";
            }
            dolog(format("Saving result to {}", p).c_str());
            ofstream ofs;
            ofs.open(p, ios::out);
            for (auto it = arrows.begin(); it != arrows.end(); ++it) {
                ofs << format("COORD:{}:{}\n", it->startPos.m_Latitude, it->startPos.m_Longitude);
                ofs << format("COORD:{}:{}\n", it->startSidePos1.m_Latitude, it->startSidePos1.m_Longitude);
                ofs << format("COORD:{}:{}\n", it->endSidePos1.m_Latitude, it->endSidePos1.m_Longitude);
                ofs << format("COORD:{}:{}\n", it->endOutPos1.m_Latitude, it->endOutPos1.m_Longitude);
                ofs << format("COORD:{}:{}\n", it->endPos.m_Latitude, it->endPos.m_Longitude);
                ofs << format("COORD:{}:{}\n", it->endOutPos2.m_Latitude, it->endOutPos2.m_Longitude);
                ofs << format("COORD:{}:{}\n", it->endSidePos2.m_Latitude, it->endSidePos2.m_Longitude);
                ofs << format("COORD:{}:{}\n", it->startSidePos2.m_Latitude, it->startSidePos2.m_Longitude);
                ofs << "COORDPOLY:100" << endl;
                ofs << endl;

            }
            ofs.close();
            return true;
        }
        return false;
    }

    void CTmRadar::OnAsrContentToBeClosed() {

    }

    void CTmRadar::dolog(const char *msg) {
        this->GetPlugIn()->DisplayUserMessage(this->GetPlugIn()->GetPlugInName(), this->GetPlugIn()->GetPlugInName(),
                                              msg,
                                              false, false, false, false, false);
    }

    CTmRadar::CTmRadar() {

    }

    void CTmRadar::OnButtonDownScreenObject(int ObjectType, const char *sObjectId, POINT Pt, RECT Area, int Button) {
        if (!drawing)return;
        if (drawing == 1) {
            currentArrow->startPos = CPosition(ConvertCoordFromPixelToPosition(Pt));
            drawing = 2;
            dolog(format("start position is {},{}", currentArrow->startPos.m_Latitude,
                         currentArrow->startPos.m_Longitude).c_str());
        } else if (drawing == 2) {
            currentArrow->endPos = CPosition(ConvertCoordFromPixelToPosition(Pt));
            drawing = 3;
            dolog(format("end position is {},{}", currentArrow->endPos.m_Latitude,
                         currentArrow->endPos.m_Longitude).c_str());
        } else {
            arrows.push_back(*currentArrow);
            drawing = 0;
            dolog(format("size = {}", size).c_str());
        }
    }

    void CTmRadar::OnRefresh(HDC hDC, int Phase) {
        if (Phase == REFRESH_PHASE_AFTER_TAGS) {
            if (drawing) AddScreenObject(0, "Drawing", this->GetRadarArea(), false, "Hello World");
            if (drawing == 2) {
                RECT radarRange = GetRadarArea();
                POINT startPosPix = ConvertCoordFromPositionToPixel(currentArrow->startPos);
                POINT endPosPix = ConvertCoordFromPositionToPixel(currentArrow->endPos);
                POINT endOutPix1 = ConvertCoordFromPositionToPixel(currentArrow->endOutPos1);
                POINT endOutPix2 = ConvertCoordFromPositionToPixel(currentArrow->endOutPos2);
                if (PtInRect(&radarRange, endOutPix1) && PtInRect(&radarRange, endOutPix2)) {
                    MoveToEx(hDC, startPosPix.x, startPosPix.y, NULL);
                    LineTo(hDC, endPosPix.x, endPosPix.y);
                    LineTo(hDC, endOutPix1.x, endOutPix1.y);
                    MoveToEx(hDC, endPosPix.x, endPosPix.y, NULL);
                    LineTo(hDC, endOutPix2.x, endOutPix2.y);
                }

            } else if (drawing == 3) {
                POINT startPosPix = ConvertCoordFromPositionToPixel(currentArrow->startPos);
                POINT endPosPix = ConvertCoordFromPositionToPixel(currentArrow->endPos);
                POINT endOutPix1 = ConvertCoordFromPositionToPixel(currentArrow->endOutPos1);
                POINT endOutPix2 = ConvertCoordFromPositionToPixel(currentArrow->endOutPos2);
                POINT startSidePix1 = ConvertCoordFromPositionToPixel(currentArrow->startSidePos1);
                POINT startSidePix2 = ConvertCoordFromPositionToPixel(currentArrow->startSidePos2);
                POINT endSidePix1 = ConvertCoordFromPositionToPixel(currentArrow->endSidePos1);
                POINT endSidePix2 = ConvertCoordFromPositionToPixel(currentArrow->endSidePos2);
                RECT radarRange = GetRadarArea();
                if (PtInRect(&radarRange, endOutPix1) && PtInRect(&radarRange, endOutPix2) &&
                    PtInRect(&radarRange, endSidePix1) && PtInRect(&radarRange, endSidePix2)) {
                    MoveToEx(hDC, startPosPix.x, startPosPix.y, NULL);
                    LineTo(hDC, startSidePix1.x, startSidePix1.y);
                    LineTo(hDC, endSidePix1.x, endSidePix1.y);
                    LineTo(hDC, endOutPix1.x, endOutPix1.y);
                    LineTo(hDC, endPosPix.x, endPosPix.y);
                    LineTo(hDC, endOutPix2.x, endOutPix2.y);
                    LineTo(hDC, endSidePix2.x, endSidePix2.y);
                    LineTo(hDC, startSidePix2.x, startSidePix2.y);
                    LineTo(hDC, startPosPix.x, startPosPix.y);
                }
            }
            for (std::list<Arrow>::iterator it = arrows.begin(); it != arrows.end(); ++it) {
                POINT startPosPix = ConvertCoordFromPositionToPixel(it->startPos);
                POINT endPosPix = ConvertCoordFromPositionToPixel(it->endPos);
                POINT endOutPix1 = ConvertCoordFromPositionToPixel(it->endOutPos1);
                POINT endOutPix2 = ConvertCoordFromPositionToPixel(it->endOutPos2);
                POINT startSidePix1 = ConvertCoordFromPositionToPixel(it->startSidePos1);
                POINT startSidePix2 = ConvertCoordFromPositionToPixel(it->startSidePos2);
                POINT endSidePix1 = ConvertCoordFromPositionToPixel(it->endSidePos1);
                POINT endSidePix2 = ConvertCoordFromPositionToPixel(it->endSidePos2);
                MoveToEx(hDC, startPosPix.x, startPosPix.y, NULL);
                LineTo(hDC, startSidePix1.x, startSidePix1.y);
                LineTo(hDC, endSidePix1.x, endSidePix1.y);
                LineTo(hDC, endOutPix1.x, endOutPix1.y);
                LineTo(hDC, endPosPix.x, endPosPix.y);
                LineTo(hDC, endOutPix2.x, endOutPix2.y);
                LineTo(hDC, endSidePix2.x, endSidePix2.y);
                LineTo(hDC, startSidePix2.x, startSidePix2.y);
                LineTo(hDC, startPosPix.x, startPosPix.y);
            }
        }
    }

    void CTmRadar::OnOverScreenObject(int ObjectType, const char *sObjectId, POINT Pt, RECT Area) {
        if (drawing == 2) {
            currentArrow->endPos = CPosition(ConvertCoordFromPixelToPosition(Pt));
            POINT startPosPix = ConvertCoordFromPositionToPixel(currentArrow->startPos);
            POINT endPosPix = ConvertCoordFromPositionToPixel(currentArrow->endPos);
            int argu = 30;
            float angle =
                    atan((float) (endPosPix.y - startPosPix.y) / (float) (endPosPix.x - startPosPix.x)) * 180.0 / PI;
            if (angle < 0) {
                angle = endPosPix.y > startPosPix.y ? angle + 180 : angle + 360;
            } else {
                angle = endPosPix.y > startPosPix.y ? angle : angle + 180;
            }
            currentArrow->endOutPos1 = ConvertCoordFromPixelToPosition(
                    POINT(endPosPix.x - argu * cos((angle + 45) * PI / 180),
                          endPosPix.y - argu * sin((angle + 45) * PI / 180)));
            currentArrow->endOutPos2 = ConvertCoordFromPixelToPosition(
                    POINT(endPosPix.x - argu * cos((angle - 45) * PI / 180),
                          endPosPix.y - argu * sin((angle - 45) * PI / 180)));
        } else if (drawing == 3) {
            POINT startPosPix = ConvertCoordFromPositionToPixel(currentArrow->startPos);
            POINT endPosPix = ConvertCoordFromPositionToPixel(currentArrow->endPos);
            POINT mousePix = Pt;
            POINT startSidePix1;
            POINT startSidePix2;
            POINT endSidePix1;
            POINT endSidePix2;
            POINT endOutPix1;
            POINT endOutPix2;
            POINT supEndPix;
            float angle =
                    atan((float) (endPosPix.y - startPosPix.y) / (float) (endPosPix.x - startPosPix.x)) * 180.0 / PI;
            if (angle < 0) {
                angle = endPosPix.y > startPosPix.y ? angle + 180 : angle + 360;
            } else {
                angle = endPosPix.y > startPosPix.y ? angle : angle + 180;
            }
            size = abs(((endPosPix.y - mousePix.y) * cos(angle * PI / 180)) -
                       ((endPosPix.x - mousePix.x) * sin(angle * PI / 180)));
            double dirX = sin(angle * PI / 180);
            double dirY = cos(angle * PI / 180);
            startSidePix1.x = startPosPix.x + 0.6*size * dirX;
            startSidePix1.y = startPosPix.y - 0.6*size * dirY;
            startSidePix2.x = startPosPix.x - 0.6*size * dirX;
            startSidePix2.y = startPosPix.y + 0.6*size * dirY;
            double supX = cos(angle * PI / 180);
            double supY = sin(angle * PI / 180);
            supEndPix.x = endPosPix.x - 2 * size * supX;
            supEndPix.y = endPosPix.y - 2 * size * supY;
            endSidePix1.x = supEndPix.x + 0.6*size * dirX;
            endSidePix1.y = supEndPix.y - 0.6*size * dirY;
            endSidePix2.x = supEndPix.x - 0.6*size * dirX;
            endSidePix2.y = supEndPix.y + 0.6*size * dirY;
            endOutPix1.x = supEndPix.x + 2 * size * dirX;
            endOutPix1.y = supEndPix.y - 2 * size * dirY;
            endOutPix2.x = supEndPix.x - 2 * size * dirX;
            endOutPix2.y = supEndPix.y + 2 * size * dirY;
            currentArrow->startSidePos1 = ConvertCoordFromPixelToPosition(startSidePix1);
            currentArrow->startSidePos2 = ConvertCoordFromPixelToPosition(startSidePix2);
            currentArrow->endOutPos1 = ConvertCoordFromPixelToPosition(endOutPix1);
            currentArrow->endOutPos2 = ConvertCoordFromPixelToPosition(endOutPix2);
            currentArrow->endSidePos1 = ConvertCoordFromPixelToPosition(endSidePix1);
            currentArrow->endSidePos2 = ConvertCoordFromPixelToPosition(endSidePix2);
        }
        if (drawing) {
            RequestRefresh();
        }
    }
} // EuroScopePlugIn