
	

	public static String readLine(int cpu) throws IOException {
		String filename = "";
		if (cpu == modem){//射频
		    filename = "/sys/devices/virtual/thermal/thermal_zone0/temp";
		}else if (cpu == cpu0_cpu2){
		    filename = "/sys/devices/virtual/thermal/thermal_zone3/temp";
		}else if (cpu == cpu1_cpu3){
		    filename = "/sys/devices/virtual/thermal/thermal_zone4/temp";
		}else if (cpu == qdsp_cam){//cam数据处理
		    filename = "/sys/devices/virtual/thermal/thermal_zone1/temp";
		}
		BufferedReader reader = new BufferedReader(new FileReader(filename), 256);
		try {
		    return reader.readLine() + "℃";
		} finally {
		    reader.close();
		}
	    }
