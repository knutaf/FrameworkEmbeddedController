/* Copyright (c) 2013 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Battery charging parameters and constraints
 */

#ifndef __CROS_EC_BATTERY_H
#define __CROS_EC_BATTERY_H

#include "common.h"

/* Stop charge when charging and battery level >= this percentage */
#define BATTERY_LEVEL_FULL		100

/* Tell host we're charged when battery level >= this percentage */
#define BATTERY_LEVEL_NEAR_FULL		 97

/*
 * Send battery-low host event when discharging and battery level <= this level
 */
#define BATTERY_LEVEL_LOW		 10

/*
 * Send battery-critical host event when discharging and battery level <= this
 * level.
 */
#define BATTERY_LEVEL_CRITICAL		  5

/*
 * Shut down main processor and/or hibernate EC when discharging and battery
 * level < this level.
 */
#define BATTERY_LEVEL_SHUTDOWN		  3

/* Battery parameters */
struct batt_params {
	int temperature;      /* Temperature in 0.1 K */
	int state_of_charge;  /* State of charge (percent, 0-100) */
	int voltage;          /* Battery voltage (mV) */
	int current;          /* Battery current (mA) */
	int desired_voltage;  /* Charging voltage desired by battery (mV) */
	int desired_current;  /* Charging current desired by battery (mA) */
};

/* Working temperature ranges in degrees C */
struct battery_temperature_ranges {
	int8_t start_charging_min_c;
	int8_t start_charging_max_c;
	int8_t charging_min_c;
	int8_t charging_max_c;
	int8_t discharging_min_c;
	int8_t discharging_max_c;
};
extern const struct battery_temperature_ranges bat_temp_ranges;

/* Battery constants */
struct battery_info {
	/* Design voltage in mV */
	int voltage_max;
	int voltage_normal;
	int voltage_min;
	/* Pre-charge current in mA */
	int precharge_current;
};

/**
 * Return vendor-provided battery constants.
 */
const struct battery_info *battery_get_info(void);

/**
 * Modify battery parameters to match vendor charging profile.
 *
 * @param batt		Battery parameters to modify
 */
void battery_vendor_params(struct batt_params *batt);

/**
 * Attempt communication with the battery.
 *
 * @return non-zero if the battery responds.
 */
int battery_is_connected(void);

/**
 * Get battery mode.
 *
 * See MODE_* constants in smart_battery.h
 *
 * @param mode		Destination for current mode.
 * @return non-zero if error.
 */
int battery_get_mode(int *mode);

/**
 * Set battery mode.
 *
 * See MODE_* constants in smart_battery.h
 *
 * @param mode		New mode.
 * @return non-zero if error.
 */
int battery_set_mode(int mode);

/**
 * Check if battery is reporting capacity in 10 mW units.
 *
 * @param val		Destination for capacity units; set zero if mAh or
 *			non-zero if 10 mW.
 * @return non-zero if error.
 */
int battery_is_in_10mw_mode(int *val);

/**
 * Set battery capacity units.
 *
 * @param enabled	Set mode to mAh (=0) or 10 mW (=1)
 * @return non-zero if error.
 */
int battery_set_10mw_mode(int enabled);

/**
 * Read battery temperature.
 *
 * @param deci_kelvin	Destination for battery temperature in units of 0.1 K
 * @return non-zero if error.
 */
int battery_temperature(int *deci_kelvin);

/**
 * Read battery voltage.
 *
 * @param voltage	Destination for voltage in mW
 * @return non-zero if error.
 */
int battery_voltage(int *voltage);

/**
 * Read nominal voltage battery is designed to supply.
 *
 * @param voltage	Destination for voltage in mW
 * @return non-zero if error.
 */
int battery_design_voltage(int *voltage);

/**
 * Read charging voltage desired by battery.
 *
 * @param voltage	Destination for voltage in mV.
 * @return non-zero if error.
 */
int battery_desired_voltage(int *voltage);

/**
 * Read battery discharging current.
 *
 * @param current	Destination for discharge current in mA; negative
 *			value indicates charging.
 * @return non-zero if error.
 */
int battery_current(int *current);

/**
 * Read averaged battery discharging current.
 *
 * @param current	Destination for discharge current in mA; negative
 *			value indicates charging.
 * @return non-zero if error.
 */
int battery_average_current(int *current);

/**
 * Read charging current desired by battery.
 *
 * @param current	Destination for current in mA.
 * @return non-zero if error.
 */
int battery_desired_current(int *current);

/**
 * Read battery relative state of charge.
 *
 * @param percent	Destination for charge in percent
 * @return non-zero if error.
 */
int battery_state_of_charge(int *percent);

/**
 * Read absolute state of charge.
 *
 * @param percent	Destination for charge in percent
 * @return non-zero if error.
 */
int battery_state_of_charge_abs(int *percent);

/**
 * Read battery remaining capacity.
 *
 * @param capacity	Destination for capacity; units are mAh or 10 mW,
 *			depending on battery_is_in_10mw_mode().
 * @return non-zero if error.
 */
int battery_remaining_capacity(int *capacity);

/**
 * Read battery full charge capacity.
 *
 * @param capacity	Destination for capacity; units are mAh or 10 mW,
 *			depending on battery_is_in_10mw_mode().
 * @return non-zero if error.
 */
int battery_full_charge_capacity(int *capacity);

/**
 * Read the nominal capacity the battery is designed to supply when new.
 *
 * @param capacity	Destination for capacity; units are mAh or 10 mW,
 *			depending on battery_is_in_10mw_mode().
 * @return non-zero if error.
 */
int battery_design_capacity(int *capacity);

/**
 * Read time in minutes left when discharging.
 *
 * @param capacity	Destination for remaining time in minutes.
 * @return non-zero if error.
 */
int battery_time_to_empty(int *minutes);

/**
 * Read run time in minutes left when discharging.
 *
 * @param capacity	Destination for remaining time in minutes.
 * @return non-zero if error.
 */
int battery_run_time_to_empty(int *minutes);

/**
 * Read time in minutes left to full capacity when charging.
 *
 * @param capacity	Destination for remaining time in minutes.
 * @return non-zero if error.
 */
int battery_time_to_full(int *minutes);

/**
 * Calculate battery time in minutes, under an assumed current.
 *
 * @param rate		Current to use for calculation, in mA.
 *			If > 0, calculates charging time; if < 0, calculates
 *			discharging time; 0 is invalid and sets minutes=0.
 * @param minutes	Destination for calculated time in minutes.
 * @return non-zero if error.
 */
int battery_time_at_rate(int rate, int *minutes);

/**
 * Check if battery allows charging.
 *
 * @param allowed	Non-zero if charging allowed; zero if not allowed.
 * @return non-zero if error.
 */
int battery_charging_allowed(int *allowed);

/**
 * Read battery status.
 *
 * @param status	Destination for status; see STATUS_* in smart_battery.h.
 * @return non-zero if error.
 */
int battery_status(int *status);

/**
 * Read battery charge cycle count.
 *
 * @param count		Destination for count.
 * @return non-zero if error.
 */
int battery_cycle_count(int *count);

/**
 * Read battery serial number.
 *
 * @param serial	Destination for serial number.
 * @return non-zero if error.
 */
int battery_serial_number(int *serial);

/**
 * Read manufacturer name.
 *
 * @param dest		Destination buffer.
 * @param size		Length of destination buffer in chars.
 * @return non-zero if error.
 */
int battery_manufacturer_name(char *dest, int size);

/**
 * Read device name.
 *
 * @param dest		Destination buffer.
 * @param size		Length of destination buffer in chars.
 * @return non-zero if error.
 */
int battery_device_name(char *dest, int size);

/**
 * Read battery type/chemistry.
 *
 * @param dest		Destination buffer.
 * @param size		Length of destination buffer in chars.
 * @return non-zero if error.
 */
int battery_device_chemistry(char *dest, int size);

/**
 * Read device manufacture date.
 *
 * @param year		Destination for year
 * @param month		Destination for month
 * @param day		Destination for day
 * @return non-zero if error.
 */
int battery_manufacturer_date(int *year, int *month, int *day);

#endif /* __CROS_EC_BATTERY_H */

