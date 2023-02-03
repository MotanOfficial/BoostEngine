import psutil

# Get set of all active processes
processes = set()
for process in psutil.process_iter():
    try:
        if not process.name().startswith("svchost"):
            processes.add(process.name())
    except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
        pass

# Get list of all active services
services = []
for service in psutil.win_service_iter():
    if service.status() == "running":
        services.append(service.name())

# Write output to a text file
with open("BoostEngineHelper.txt", "w") as file:
    file.write("BoostEngineHelper Version 1.0\n")
    file.write("\n=====================\n")
    file.write("Active processes:\n")
    for process in processes:
        file.write(process + "\n")

    file.write("\n=====================\n")
    file.write("\nActive services:\n")
    for service in services:
        file.write(service + "\n")

    file.write("\n=====================\n")
    file.write("While closing processes and srvices, please be careful not to close any important processes or services...")

# Print message to confirm that output has been written to a file
print("Processes, services, and background apps have been written to a file.")