#include <avr/io.h>
#include "mc25aa.h"

#define READ  0x03
#define WRITE 0x02
#define WREN  0x06
#define WRDI  0x04
#define RDSR  0x05
#define WRSR  0x01
#define PE    0x42
#define SER   0xD8
#define CE    0xC7
#define RDID  0xAB
#define DPD   0xB9

inline void mem_select()
{
  PORTD &= ~0x10;
}

inline void mem_unselect()
{
    PORTD |= 0x10;
}

void spi_init()
{
    /* SS(USED) */
    //DDRD |= 0x01;
    DDRD |= 0x10;
    mem_unselect();

    /*           MOSI    SS(UNUSED)    SCK */
    DDRB |= (1 << DDB3) | (1 << DDB2) | (1 << DDB5);
    DDRB &= ~((1 << DDB4));

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);       
}

inline void _spi_wait_for_spif()
{
    while(!(SPSR & (1 << SPIF)));
}

void _spi_tx(char b)
{
    SPDR = b & 0xFF;

    _spi_wait_for_spif();
}

void _spi_tx_addr(uint32_t addr)
{
    _spi_tx((addr & 0x00FF0000U) >> 16);
    _spi_tx((addr & 0x0000FF00U) >> 8);
    _spi_tx((addr & 0x000000FFU));
}

void mc25aa_dpd()
{
    mem_select();

    _spi_tx(DPD);

    mem_unselect();
}

char mc25aa_rdid()
{
    mem_select();    

    _spi_tx(RDID);

    _spi_tx(0x00);
    _spi_tx(0x00);    
    _spi_tx(0x00);
    _spi_tx(0x00);
    
    mem_unselect();

    return SPDR;
}

void mc25aa_wren()
{
    mem_select();

    _spi_tx(WREN);

    mem_unselect();
}

void mc25aa_wrdi()
{
    mem_select();

    _spi_tx(WRDI);

    mem_unselect();
}

char mc25aa_rdsr()
{
    mem_select();

    _spi_tx(RDSR);
    _spi_tx(0x00);

    mem_unselect();

    return SPDR;
}

void mc25aa_wrsr(char b)
{
    mem_select();

    _spi_tx(WRSR);
    _spi_tx(b);

    mem_unselect();
}    

char mc25aa_rbyte(uint32_t addr)
{
    mem_select();

    _spi_tx(READ);
    _spi_tx_addr(addr);
    _spi_tx(0x00);

    mem_unselect();

    return SPDR;
}

void mc25aa_wpage(uint32_t addr, uint32_t len, char *data)
{
    mem_select();

    _spi_tx(WRITE);
    _spi_tx_addr(addr);

    uint32_t i;
    for(i = 0; i < len; i++) {
	_spi_tx(*data);
	data++;
    }
    
    mem_unselect();
}

void mc25aa_wbyte(uint32_t addr, char data)
{
    mc25aa_wpage(addr, 1, &data);
}

void wait_while_wip()
{
    while (mc25aa_rdsr() & 0x01);
}
     
void mc25aa_page_erase(uint32_t addr)
{
    mc25aa_wren();

    mem_select();
    
    _spi_tx(PE);
    _spi_tx_addr(addr);

    mem_unselect();
}

void mc25aa_sector_erase(uint32_t addr)
{
    mc25aa_wren();

    mem_select();
    
    _spi_tx(SER);
    _spi_tx_addr(addr);

    mem_unselect();
}

void mc25aa_chip_erase()
{
    mc25aa_wren();

    mem_select();

    _spi_tx(CE);
    
    mem_unselect();
}

/*
void main()
{
    delay_ms(1000);

    spi_init();

    mc25aa_wren();
    mc25aa_wbyte(60, 55);

    wait_while_wip();

    mc25aa_rbyte(60);

    delay_ms(10);

    mc25aa_wren();
    mc25aa_chip_erase(60);
    delay_ms(1);
    wait_while_wip();

    delay_ms(10);

    mc25aa_rbyte(60);

}
*/
