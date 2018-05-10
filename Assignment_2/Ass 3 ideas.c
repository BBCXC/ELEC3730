// Reduce display width and height to parameter.
// 10 second buffer
// start stop button
// null all of the values first
//
// samples per second 1kHz
// use dma to read in data
// 16bit double
// 10 seconds saved
// ~= 20kb buffer
// too much data for the screen
// sample at higher rate
// compress data to smaller amount
//(maybe add auto scale / zoom feature)
// auto scale on the buffer redo(max buffer value re draw? might be slow)
// Implementation
// Circular buffer (A1 has one)
// 2 options
// scroll information
// or window line that moves left to right
//
// noise?
//
// on stop data, store data to sd card. (when reading store the 16bit data and
// then convert when displaying)
//
// SD Implementation
// FATFS library
// store button
// load button
//
// stop display
// store
//
// load specific file
//
// store file as txt(csv)
// comma separated
// column for time
// column for values
// similar to silly scope
// information at top of file
// date
// instrument
// data parameters
// column names
// scale factor

// Extra features
// timestamp (stored) ASAP(better correlation)
// overwrite files on sd card
// scroll menu?? on sd card
// might be hard

// console features
// acess sd card
// display files stored
// delete files
// read file and display on LCD
// directory
// (ls cd)
// (cd.. rm -rf*)
// (cp files)

// Analog input
// voltage level
// ADC
//

// Interrupt
//