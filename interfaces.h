/*
 * Hardware serial interfaces.
 */

// A struct that maps onto the MSP430's serial hardware's MMIO registers
typedef struct {
	volatile uint8_t CTL0;
	volatile uint8_t CTL1;
	volatile uint8_t BR0;
	volatile uint8_t BR1;
	volatile uint8_t MCTL;
	volatile uint8_t STAT;
	volatile uint8_t RXBUF;
	volatile uint8_t TXBUF;
} usci_mem_t;

// A struct that represents the state for a hardware serial (USCI) module
typedef struct {
	volatile usci_mem_t *UCxx;
	volatile uint8_t *buffer_head;
	volatile uint8_t *buffer_tail;
	volatile uint16_t TXIFG;
	volatile uint8_t buffer[USCI_BUFFER_SIZE];
	volatile bool data_ready;
} usci_t;


// Hardware serial (USCI) module intances
extern usci_t *USCI_A0;
extern usci_t *USCI_B0;

/**
 * Receive data from hardware serial (USCI) module.
 *
 * By default recv() will read up to `n' bytes and return if no more data is
 * available.
 *
 * Options:
 *
 * USCI_BLOCKING   recv() blocks until exactly `n' bytes are read into the
 *                 buffer
 * USCI_DELIM      recv() returns as soon as it writes `delim' into the buffer
 * USCI_DEFAULT    recv() operates according to its default behavior
 *
 * @param usci		Pointer to USCI instance
 * @param buffer	Byte buffer data will be read into
 * @param n			Number of bytes to read
 * @param delim		Speficied delimiter.  If not using USCI_DELIM, this field
 *                  is ignored.
 * @param options	Options field specified by OR'ing together one or more
 *              	options.
 *
 * @returns number of bytes read
 */
ssize_t recv ( usci_t *usci, uint8_t *buffer, size_t n, uint8_t delim, uint8_t options );

/**
 * Send data over hardware serial (USCI) module.
 *
 * @param usci		Pointer to USCI instance
 * @param data		Pointer to array of bytes to send
 * @param len		Length of byte array
 */
void send ( usci_t *usci, const uint8_t *data, size_t len );

/**
 * Flush hardware transmit buffer.
 *
 * Blocks until the hardware serial (USCI) module has finished sending
 * all data in its internal buffer.
 *
 * @param usci		Pointer to USCI instance
 */
void flush ( usci_t *usci );

/**
 * Clear data in USCI instance's software buffer.
 *
 * Data is transparently read into the USCI instance's buffer via an ISR.
 * Calling clear() resets this buffer.
 */
void clear ( usci_t *usci );

/**
 * Initialize SPI.
 *
 * Sets up USCI_B0 for SPI mode.  By default SCLK is 1/8*SMCLK.
 *
 * Options:
 *
 * Currently there are no options implemented.  Ideally this would let you
 * set parameters like clock speed, polarity, and phase.
 *
 * @param options	SPI settings
 */
void SPI_init ( uint8_t options );

/**
 * Send data over SPI.
 *
 * @param data		Byte array of bytes to send
 * @param len		Length of byte aray
 */
void SPI_send ( uint8_t *data, size_t len );

/**
 * Receive data from SPI.
 *
 * This is effectively a call to recv() with USCI_B0 in SPI mode.
 *
 * Options:  see recv()
 *
 * @param buffer	Pointer to buffer to read data into
 * @param n			Number of bytes to read
 * @param delim		Delimiter used for USCI_DELIM options
 * @param options	See recv()
 */
ssize_t SPI_recv ( uint8_t *buffer, size_t n, uint8_t delim, uint8_t options );


/**
 * Intialze UART.
 *
 * Sets up USCI_A0 for UART mode.  By default the baud rate is set to 9600.
 *
 * Options:
 *
 * UART_600_BAUD    Configures UART as 600 baud
 * UART_9600_BAUD   Configures UART as 9600 baud (default)
 * UART_128000_BAUD Configures UART as 128000 baud
 *
 * WARNING:  These baud vaules are only valid for a clock speed of 16MHz!
 *
 * @param options	Sets UART parameters
 */
void UART_init ( uint8_t options );

/**
 * Send data over UART.
 *
 * @param data		Byte array of bytes to send
 * @param len		Length of byte aray
 */
void UART_send ( uint8_t *data, size_t len );

/**
 * Receive data from UART.
 *
 * This is effectively a call to recv() with USCI_A0 in UART mode.
 *
 * Options:  see recv()
 *
 * @param buffer	Pointer to buffer to read data into
 * @param n			Number of bytes to read
 * @param delim		Delimiter used for USCI_DELIM options
 * @param options	See recv()
 */
ssize_t UART_recv ( uint8_t *buffer, size_t n, uint8_t delim, uint8_t options );



/*
 * CS5490 AC power meter interface.
 */


/**
 * Initialize power meter.
 *
 * Initializes serial interface, configures meter, and loads stored
 * calibration values.
 */
void METER_init ( );

/**
 * Begin metering power.
 *
 * An interrupt is generated when new data is ready (roughly every second).
 * This value is automatically read via an ISR.
 */
void METER_begin ( );

/**
 * Read last power measurement.
 *
 * Returns the raw power value last read from the meter.  This value requires
 * further processing to compute the value in watts.
 *
 * Calibration guide: http://www.cirrus.com/en/pubs/appNote/AN366REV2.pdf
 */
uint32_t METER_read ( );



/*
 * Solid state relay interface.
 */


/**
 * Initialize the pin controlling the SSR.
 */
void RELAY_init ( );

/**
 * Power on the relay.
 */
void RELAY_on ( );

/**
 * Power off the relay.
 */
void RELAY_off ( );


/*
 * RFM12-B wireless interface.
 */

/**
 * Initialize the RFM12-B module.
 */
void RF0_init ( );

/**
 * Transmit data over RFM12-B interface.
 *
 * @param dest		Destination device's network address
 * @param opcode	Packet type (eg. POWER_ON)
 * @param data		Pointer to byte array to transmit
 * @param len		Length of byte array
 */
void RF0_tx ( uint8_t dest, uint8_t opcode, uint8_t *data, size_t len );

/**
 * Receive data over RF interfaces.
 *
 * This call blocks until exactly `n' bytes have been read.
 *
 * @param buffer	Byte buffer to receive data
 * @param n			Number of bytes to receive
 */
void RF0_rx ( uint8_t *buffer, size_t n );

/**
 * Send an ACK to the coordinator.
 *
 * @param msg_id	Packet number we are acknowledging
 */
void RF0_send_ack ( uint8_t msg_id );


/**
 * Send last power measurement to coordinator.
 *
 * @param power		Power measurement to send
 * @param msg_id	Packet number of REQ_POWER packet we are responding to
 */
void RF0_send_power ( uint32_t power, uint8_t msg_id );


