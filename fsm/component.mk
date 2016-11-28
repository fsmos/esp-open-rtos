INC_DIRS += $(fsm_ROOT)include

# args for passing into compile rule generation
fsm_SRC_DIR = $(fsm_ROOT)

$(eval $(call component_compile_rules,fsm))