if(SMTG_AAX_SDK_PATH AND SMTG_ADD_VSTGUI)
  include(AddAAXLibrary)
  set(
    base_sources
    ${SDK_ROOT}/public.sdk/source/vst/basewrapper/basewrapper.sdk.cpp
  )
  set(
    Tape50_aax_sources
    source/aax.cpp
    ../Tape50/include/buffer.h
    ../Tape50/include/constants.h
    ../Tape50/include/controller.h
    ../Tape50/include/id.h
    ../Tape50/include/parameter.h
    ../Tape50/include/processor.h
    ../Tape50/include/version.h
    ../Tape50/source/buffer.cpp
    ../Tape50/source/controller.cpp
    ../Tape50/source/factory.cpp
    ../Tape50/source/parameter.cpp
    ../Tape50/source/processor.cpp
  )
  set(Tape50_sources ${Tape50_aax_sources} ${base_sources})
  set(target Tape50aax)
  smtg_add_aaxplugin(${target} ${Tape50_sources})
  target_include_directories(${target} PUBLIC ../Tape50/include)
  target_include_directories(${target} PUBLIC ../Tape50/source)
  target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
  target_link_libraries(${target} PRIVATE base sdk vstgui_support aaxwrapper)

  smtg_add_vst3_resource(${target} "../Tape50/resource/main.uidesc")

  smtg_add_vst3_resource(${target} "../Tape50/image/knob_1x.png")
  smtg_add_vst3_resource(${target} "../Tape50/image/knob_2x.png")
  smtg_add_vst3_resource(${target} "../Tape50/image/knob_3x.png")

  if(SMTG_MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/../Tape50/resource/Info.plist" PREPROCESS)
  elseif(SMTG_WIN)
    target_sources(${target} PRIVATE ../Tape50/resource/Plugin.rc)
  endif()
endif()
