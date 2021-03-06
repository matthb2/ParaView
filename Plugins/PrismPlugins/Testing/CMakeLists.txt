INCLUDE(ParaViewTestingMacros)

SET (PRISM_TESTS_WITH_BASELINES
  ${CMAKE_CURRENT_SOURCE_DIR}/PrismMultipleSurfaces.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/PrismSelection.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/PrismView.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/PrismScaleView.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/PrismScaleView1.xml
)

SET (PrismMultipleSurfaces_BREAK TRUE)
SET (PrismSelection_BREAK TRUE)
SET (PrismView_BREAK TRUE)
SET (PrismScaleView_BREAK TRUE)
SET (PrismScaleView1_BREAK TRUE)

set(PrismMultipleSurfaces_USE_OLD_PANELS TRUE)
set(PrismSelection_USE_OLD_PANELS TRUE)
set(PrismView_USE_OLD_PANELS TRUE)
set(PrismScaleView_USE_OLD_PANELS TRUE)
set(PrismScaleView1_USE_OLD_PANELS TRUE)

IF (PARAVIEW_DATA_ROOT AND BUILD_SHARED_LIBS)
  add_client_tests("Prism"
    BASELINE_DIR ${PARAVIEW_DATA_ROOT}/Baseline
    TEST_SCRIPTS ${PRISM_TESTS_WITH_BASELINES}
  )
ENDIF (PARAVIEW_DATA_ROOT AND BUILD_SHARED_LIBS)
