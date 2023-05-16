//
// Created by HarryXiong on 2023/5/15.
//

#ifndef TOPSKYARROWMARKER_CTMRADAR_H
#define TOPSKYARROWMARKER_CTMRADAR_H

#include "EuroScopePlugIn.h"

namespace EuroScopePlugIn {

    class CTmRadar :public CRadarScreen{
    public:
        CTmRadar();
        void OnRefresh(HDC hDC, int Phase) override;
        bool OnCompileCommand ( const char * sCommandLine ) override;
        void OnAsrContentToBeClosed() override;
        void dolog(const char*);
        void OnButtonDownScreenObject(int ObjectType, const char *sObjectId, POINT Pt, RECT Area, int Button) override;
        bool drawing = false;
        int size = 0;
        float angle=0;
        CPosition startPos;
        CPosition endPos;
        void OnOverScreenObject(int ObjectType, const char *sObjectId, POINT Pt, RECT Area) override;
        CPosition startSidePos1;
        CPosition startSidePos2;
        CPosition endSidePos1;
        CPosition endSidePos2;
        CPosition endOutPos1;
        CPosition endOutPos2;
    };

} // EuroScopePlugIn

#endif //TOPSKYARROWMARKER_CTMRADAR_H
