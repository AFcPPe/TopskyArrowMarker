
#include "Plugin.h"
#include "CTmRadar.h"
#include <string>

using namespace std;

ESPlugin::ESPlugin() : EuroScopePlugIn::CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, MY_PLUGIN_NAME, MY_PLUGIN_VERSION, MY_PLUGIN_DEVELOPER, MY_PLUGIN_COPYRIGHT)
{
	string pluginName = MY_PLUGIN_NAME;
	string version = MY_PLUGIN_VERSION;
	DisplayUserMessage(pluginName.c_str(), pluginName.c_str(), ("Version " + version + " loaded").c_str(), true, false, false, false, false);


}



ESPlugin::~ESPlugin()
{


}

CRadarScreen *
ESPlugin::OnRadarScreenCreated(const char *sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved,
                               bool CanBeCreated) {
    return new CTmRadar;
}
