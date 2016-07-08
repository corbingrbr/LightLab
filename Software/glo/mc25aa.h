#ifndef MC25AA_H_
#define MC25AA_H_

inline void mem_select();
inline void mem_unselect();
inline void _spi_wait_for_spif();
void spi_init();
void _spi_tx(char b);
void _spi_tx_addr(uint32_t addr);

void mc25aa_dpd();
char mc25aa_rdid();
void mc25aa_wren();
void mc25aa_wrdi();
char mc25aa_rdsr();
void mc25aa_wrsr(char b);
char mc25aa_rbyte(uint32_t addr);
void mc25aa_wpage(uint32_t addr, uint32_t len, char *data);
void mc25aa_wbyte(uint32_t addr, char data);
void wait_while_wip();
void mc25aa_page_erase(uint32_t addr);
void mc25aa_sector_erase(uint32_t addr);
void mc25aa_chip_erase();

#endif
