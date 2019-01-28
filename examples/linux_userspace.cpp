/*
  Linux userspace test code, simple and mose code directy from the doco.
  compile like this: gcc linux_userspace.c ../bme280.c -I ../ -o bme280
  tested: Raspberry Pi.
  Use like: ./bme280 /dev/i2c-0
*/
#include "bme280.hpp"
#include "I2C.hpp"

void print_sensor_data(data& dat)
{
#ifdef BME280_FLOAT_ENABLE
  printf("temp %0.2f, p %0.2f, hum %0.2f\r\n",dat.getTemperature(), dat.getPressure(), dat.getHumidity());
#else
  printf("temp %ld, p %ld, hum %ld\r\n",dat.getTemperature(),dat.getPressure(),dat.getHumidity());
#endif
}

int8_t stream_sensor_data_forced_mode(bme280& bm)
{
  int8_t rslt;
  uint8_t settings_sel;
  settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
  rslt = bm.bme280_set_sensor_settings(settings_sel);
  printf("Temperature, Pressure, Humidity\r\n");
  /* Continuously stream sensor data */
  while (1) 
  {
    rslt = bm.bme280_set_sensor_mode(BME280_FORCED_MODE);
    /* Wait for the measurement to complete and print data @25Hz */
    bm.delay_ms(40);
    rslt = bm.bme280_get_sensor_data(BME280_ALL);
    print_sensor_data(bm.getData());
  }
  return rslt;
}

int main(int argc, char* argv[])
{
  I2C i2c(argv[1],"0x76");
  settings setting;
  setting.setOversamplingPressure("16X");
  setting.setOversamplingHumidity("1X");
  setting.setOversamplingTemperature("2X");
  setting.setFilterCoefficient("16");
  i2c.connect();
  bme280 bm(i2c,setting);
  bm.m_IO=&i2c;
  int8_t rslt = BME280_OK;

  rslt = bm.bme280_init();
  stream_sensor_data_forced_mode(bm);
}
