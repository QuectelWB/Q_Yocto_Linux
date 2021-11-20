#线性充电


电池对应的文件为《batterydata-qrd-l42q-4v20-3200mah.dtsi》，在msm8909-qrd-skue-cb03.dtsi文件中被引用，开发板采用的是
	
	batterydata-qrd-l45q-4v35-1800mah.dtsi

电池曲线文件说明文档见
	
	kernel\Documentation\devicetree\bindings\batterydata\batterydata.txt文件
	
	Battery Data is a collection of batteryprofile data made available to the QPNP Charger and BMS drivers via devicetree.
	
	电池数据是电池配置数据的集合，通过设备树供QPNP充电器和BMS驱动使用。

	
qcom,battery-data node required properties(电池数据节点需要的属性):

- qcom,rpull-up-kohm : The vadc pullupresistor's resistance value in kOhms.

Vadc上拉电阻的阻值

- qcom,vref-batt-therm-uv : The vadcvoltage used to make readings.
                     ForQualcomm VADCs this should be 1800000uV.
 
qcom,battery-data node optional properties:
- qcom,batt-id-range-pct : The area ofvariation between upper and lower bound
                     forwhich a given battery ID resistance is valid. This
                     valueis expressed as a percentage of the specified kohm
                     resistanceprovided by qcom,batt-id-kohm.
 
qcom,battery-data can also include anynumber of children nodes. These children
nodes will be treated as battery profiledata nodes.

<br>
Batter-data可包含任何数量的子节点，这些子节点将会作为电池配置文件节点来创建。

## batterydata-qrd-l42q-4v20-3400mah.dtsi


	qcom,qrd-l42q-4v20-3200mah-data{
		     qcom,battery-type ="qrd_l42q_4v20_3200mah";
		     qcom,batt-id-kohm = <10>;      
	The battery idresistance of the battery. It can be used as an array which could supportmultiple IDs for one battery module when the ID resistance of some batterymodules goes across several ranges.
	电池ID电阻：当一些电池模型的ID电阻在一定范围内浮动时，电池ID电阻可以作为单电池模型数组以支持多ID
		     qcom,chg-term-ua = <96000>;
	The terminationcharging current of the battery电池的结束充电电流，这里为96mA
		     qcom,default-rbatt-mohm =<120>;
	The nominalbattery resistance value蓄电池电阻值
	 
		     qcom,fcc-mah = <3200>;
	Full charge countof the battery in milliamp-hours完全充满时电池容量为3200mAh
		     qcom,max-voltage-uv =<4200000>;
	The maximum ratedvoltage of the battery电池最大的额定电压
		     qcom,rbatt-capacitive-mohm =<50>;
	The capacitiveresistance of the battery电池的电容电阻
		     qcom,v-cutoff-uv = <3400000>;
	The cutoffvoltage of the battery at which the device should shutdown gracefully
	电池的截止电压，当电池电压低于此值时设备会自动关机
	 
		qcom,fcc-temp-lut {
		      qcom,lut-col-legend = <(-20) 025 40 60>;
		      qcom,lut-data = <3306 3328 33343337 3336>;
		     };
	电池配置数据需要fcc-temp-lut子节点
	- qcom,fcc-temp-lut : An 1-dimensionallookup table node that encodes temperature to fcc lookup. The units for thislookuptable should be degrees celsius to milliamp-hours.
	一维查找表节点会将温度编码成fcc查找。该方法的单位也应该从摄氏度变成毫安时。
	- qcom,lut-col-legend : An array thatencodes the legend of the lookup table's
			     columns.The length of this array will determine the
			     lookuptable's width.
	- qcom,lut-data : An array that encodes thelookup table's data. The size of this
			     arrayshould be equal to the size of qcom,lut-col-legend
			     multipliedby 1 if it's a 1-dimensional table, or
			     thesize of qcom,lut-row-legend if it's a 2-dimensional
			     table.The data should be in a flattened row-major
			     representation.如果lut-row-legend是一个二维表，lut-data相应的是一个偏平行表示
	 
	 
	       qcom,ibat-acc-lut{
		      qcom,lut-col-legend= <(-20) 0 25>;
			qcom,lut-row-legend = <0 250500 1000 1500>,
					   <2000>;
		      qcom,lut-data= <3272 3294 3302>,
				      <2396 3274 3296>,
				    <15743206 3282>,
				    <7283036 3248>,
				    <5022772 3182>,
				    <3382311 3020>;
	 
	       };
	Lookup table optional properties:
	- qcom,lut-row-legend : An array thatencodes the legend of the lookup table's rows.
			     Ifthis property exists, then it is assumed that the
			     lookuptable is a 2-dimensional table.
	 
	       qcom,pc-temp-ocv-lut{
		      qcom,lut-col-legend= <(-20) 0 25 40 60>;
		      qcom,lut-row-legend= <100 95 90 85 80>,
					   <7570 65 60 55>,
					   <5045 40 35 30>,
					   <2520 16 13 11>,
					   <109 8 7 6>,
					   <54 3 2 1>,
					   <0>;
		       qcom,lut-data = <4191 41884183 4181 4178>,
				    <41184128 4126 4123 4120>,
				    <40704085 4081 4077 4074>,
				    <39894037 4040 4037 4032>,
				    <39423976 3992 3994 3993>,
				    <39083943 3963 3962 3959>,
				    <38743912 3934 3932 3928>,
				    <38463880 3900 3901 3898>,
				    <38223854 3862 3864 3864>,
				    <38043829 3833 3832 3830>,
				    <37903809 3813 3812 3810>,
				    <37783792 3798 3796 3794>,
				    <37663778 3785 3784 3782>,
				    <37553768 3774 3773 3770>,
				    <37423759 3763 3758 3750>,
				    <37263746 3749 3742 3730>,
				    <37083730 3732 3724 3712>,
				    <36883716 3716 3707 3696>,
				    <36723708 3699 3691 3680>,
				    <36563701 3690 3682 3670>,
				    <36463698 3688 3680 3668>,
				    <36353693 3686 3678 3667>,
				    <36233688 3684 3676 3665>,
				    <36083680 3680 3674 3662>,
				    <35903668 3674 3667 3655>,
				    <35703646 3655 3648 3634>,
				    <35443615 3619 3612 3598>,
				    <35103572 3570 3564 3551>,
				    <34663511 3506 3501 3488>,
				    <34023424 3419 3415 3405>,
				    <33003300 3300 3300 3300>;
	 
		      };
	- qcom,pc-temp-ocv-lut : A 2-dimensionallookup table node that encodes
			     temperatureand percent charge to open circuit voltage
			     lookup.The units for this lookup table should be
			     degreescelsius and percent to millivolts.
	一个包含温度和百分比的开路电压2维查找表，查找表的单位是百分比和电压的关系
	- qcom,rbatt-sf-lut : A 2-dimentionallookup table node that encodes
			     temperatureand percent charge to battery internal
			     resistance lookup. Theunits for this lookup table
			     shouldbe degrees celsius and percent to milliohms.


## msm8909-qrd-skue-cb03.dtsi

	qrd_batterydata:qcom,battery-data {
		      qcom,rpull-up-kohm= <0>;
		      qcom,vref-batt-therm= <1800000>;
		     
		      //#include"batterydata-qrd-l45q-4v35-1800mah.dtsi"
			#include"batterydata-qrd-l42q-4v20-3200mah.dtsi"
		      //#include"batterydata-qrd-skue-4v35-2000mah.dtsi"
	       };
	};
	 
	&pm8909_chg {
	       qcom,vddmax-mv= <4200>; //Target voltage of battery in mV.电池电压值
	       qcom,vddsafe-mv= <4380>;// Maximum Vdd voltage in mV，电池最高的安全电压会比4.2V会高一些。你们电池规格是上面会有的
	       qcom,vinmin-mv= <4470>;// Minimum input voltage in mV. 这个是VBUS进到充电IC的最低电压。 一般是5V， 4470是最低的值
	       qcom,batt-hot-percentage= <25>;
	       qcom,batt-cold-percentage= <80>;
	       qcom,tchg-mins= <360>;
	       qcom,bms-controlled-charging;
	       qcom,disable-vbatdet-based-recharge;
	       status= "okay";
	 
	       //qcom,use-external-charger;
	       qcom,charging-disabled;
	};

## msm-pm8909.dtsi

	pm8909_chg: qcom,charger {
			     spmi-dev-container;
			     compatible= "qcom,qpnp-linear-charger";
			     #address-cells= <1>;
			     #size-cells= <1>;
	 
			     qcom,vddmax-mv= <4200>;
			     qcom,vddsafe-mv= <4230>;//<4200>;
			     qcom,vinmin-mv= <4470>;//<4308>//init;
			     qcom,ibatsafe-ma= <1440>;
			     qcom,thermal-mitigation= <1440 720 630 0>;
			     qcom,cool-bat-decidegc= <100>;
			     qcom,warm-bat-decidegc= <450>;
			     qcom,cool-bat-mv= <4100>;
			     qcom,warm-bat-mv= <4100>;
			     qcom,ibatmax-warm-ma= <360>;
			     qcom,ibatmax-cool-ma= <360>;
			     qcom,batt-hot-percentage= <25>;
			     qcom,batt-cold-percentage= <80>;
			     qcom,tchg-mins= <232>;
			     qcom,chg-vadc= <&pm8909_vadc>;
			     qcom,chg-adc_tm= <&pm8909_adc_tm>;
	 
			     status= "disabled";
	 
			     qcom,chgr@1000{
				    reg= <0x1000 0x100>;
				    interrupts=  <0x0 0x10 0x7>,
						  <0x00x10 0x6>,
						  <0x00x10 0x5>,
						  <0x00x10 0x0>;
				    interrupt-names=      "chg-done",
							 "chg-failed",
							 "fast-chg-on",
							 "vbat-det-lo";
			     };
	 
			     qcom,bat-if@1200{
				    reg= <0x1200 0x100>;
				    interrupts=  <0x0 0x12 0x1>,
						  <0x00x12 0x0>;
				    interrupt-names=      "bat-temp-ok",
							 "batt-pres";
			     };
	 
			     qcom,usb-chgpth@1300{
				    reg= <0x1300 0x100>;
				    interrupts=  <0 0x13 0x4>,
						  <00x13 0x2>,
						  <00x13 0x1>;
				    interrupt-names=      "usb-over-temp",
							 "chg-gone",
							 "usbin-valid";
			     };
	 
			     qcom,chg-misc@1600{
				    reg= <0x1600 0x100>;
			     };
		      };



