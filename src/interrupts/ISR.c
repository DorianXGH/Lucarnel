#include "../includes/interrupts.h"
__attribute__((interrupt)) void ISR_general_handler(struct IF* interrupt_frame) {

}
__attribute__((interrupt)) void ISR_error_handler(struct IF* interrupt_frame, uint64_t error_code) {
    
}