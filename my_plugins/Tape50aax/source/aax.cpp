#include "id.h"
#include "pluginterfaces/base/futils.h"
#include "public.sdk/source/vst/aaxwrapper/aaxwrapper_description.h"

#if 1 // MIDI inputs for instruments or Fx with MIDI input
AAX_MIDI_Desc effMIDI[] = {{"Tape50", 0xffff}, {nullptr}};
#endif

AAX_Meter_Desc effMeters[] = {};

AAX_Plugin_Desc effPlugins[] = {
    {"com.tqaudio.tape50.mono", "Tape50", CCONST('D', 'L', 'Y', '1'),
     CCONST('D', 'L', 'A', '1'), 1, 1, 0, effMIDI, nullptr, effMeters, 0},
    {"com.tqaudio.tape50.stereo", "Tape50", CCONST('D', 'L', 'Y', '2'),
     CCONST('D', 'L', 'A', '2'), 2, 2, 0, effMIDI, nullptr, effMeters, 0},
    {nullptr}};

AAX_Effect_Desc effDesc = {
    "TQ Audio",
    "Tape50",
    CCONST('T', 'Q', 'A', 'U'),
    CCONST('D', 'L', 'A', 'Y'),
    "Fx",
    {0},
    1,
    nullptr,
    effPlugins,
};

int *forceLinkAAXWrapper = &AAXWrapper_linkAnchor;

AAX_Effect_Desc *AAXWrapper_GetDescription() {
  memcpy(effDesc.mVST3PluginID, (const char *)Tape50::ProcessorID,
         sizeof(effDesc.mVST3PluginID));
  return &effDesc;
}
