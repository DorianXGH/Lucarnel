#include "../includes/interrupts.h"
__attribute__((interrupt)) void ISR_general_handler(struct IFrame* interrupt_frame) {

}
__attribute__((interrupt)) void ISR_error_handler(struct IFrame* interrupt_frame, uint64_t error_code) {

}