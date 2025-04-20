const apiKey = 'e4d8d32082b9453cc14df1a9d27cf1a7'; // Replace with your OpenWeatherMap API key
const airQualityDiv = document.getElementById('air-quality-data');
const loadingIndicator = document.getElementById('loading-indicator');
const errorMessage = document.getElementById('error-message');

async function fetchAirQuality() {
    try {
        // Show loading indicator while fetching data
        loadingIndicator.style.display = 'block';
        errorMessage.style.display = 'none';
        airQualityDiv.style.display = 'none';

        // Fetch data from the API
        const response = await fetch(`https://api.openweathermap.org/data/2.5/air_pollution?lat=28.6139&lon=77.2090&appid=${apiKey}`);
        const data = await response.json();

        // Log the entire response data to check if it's coming in as expected
        console.log('API Response:', data);

        if (data && data.list && data.list.length > 0) {
            // Display the air quality data
            displayAirQuality(data);
            loadingIndicator.style.display = 'none'; // Hide loading indicator
            airQualityDiv.style.display = 'block';   // Show air quality data
        } else {
            throw new Error("No air quality data available.");
        }
    } catch (error) {
        // Log the error for debugging
        console.error('Error fetching air quality data:', error);
        
        loadingIndicator.style.display = 'none'; // Hide loading indicator
        errorMessage.style.display = 'block';    // Show error message
        errorMessage.textContent = "Failed to load air quality data. Please try again later."; // User-friendly message
    }
}

function displayAirQuality(data) {
    const aqi = data.list[0].main.aqi;
    const pollutants = data.list[0].components;

    const aqiLevels = ['Good', 'Fair', 'Moderate', 'Poor', 'Very Poor'];
    const aqiDescription = aqiLevels[aqi - 1];

    let pollutantsTable = `
        <table>
            <thead>
                <tr>
                    <th>Pollutant</th>
                    <th>Concentration (µg/m³)</th>
                    <th>Air Quality Level</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>PM2.5</td>
                    <td>${pollutants.pm2_5}</td>
                    <td class="level ${getPollutantLevelClass(pollutants.pm2_5)}">${getPollutantLevel(pollutants.pm2_5)}</td>
                </tr>
                <tr>
                    <td>PM10</td>
                    <td>${pollutants.pm10}</td>
                    <td class="level ${getPollutantLevelClass(pollutants.pm10)}">${getPollutantLevel(pollutants.pm10)}</td>
                </tr>
                <tr>
                    <td>CO</td>
                    <td>${pollutants.co}</td>
                    <td class="level ${getPollutantLevelClass(pollutants.co)}">${getPollutantLevel(pollutants.co)}</td>
                </tr>
                <tr>
                    <td>NO2</td>
                    <td>${pollutants.no2}</td>
                    <td class="level ${getPollutantLevelClass(pollutants.no2)}">${getPollutantLevel(pollutants.no2)}</td>
                </tr>
                <tr>
                    <td>SO2</td>
                    <td>${pollutants.so2}</td>
                    <td class="level ${getPollutantLevelClass(pollutants.so2)}">${getPollutantLevel(pollutants.so2)}</td>
                </tr>
                <tr>
                    <td>O3</td>
                    <td>${pollutants.o3}</td>
                    <td class="level ${getPollutantLevelClass(pollutants.o3)}">${getPollutantLevel(pollutants.o3)}</td>
                </tr>
            </tbody>
        </table>
    `;

    airQualityDiv.innerHTML = `
        <h3>Air Quality Index (AQI): ${aqiDescription} (${aqi})</h3>
        ${pollutantsTable}
    `;
}

function getPollutantLevel(value) {
    if (value < 12) return 'Good';
    if (value < 35.5) return 'Moderate';
    if (value < 55.5) return 'Unhealthy for Sensitive Groups';
    if (value < 150.5) return 'Unhealthy';
    return 'Very Unhealthy';
}

function getPollutantLevelClass(value) {
    if (value < 12) return 'Good';
    if (value < 35.5) return 'Moderate';
    if (value < 55.5) return 'Unhealthy for Sensitive Groups';
    if (value < 150.5) return 'Unhealthy';
    return 'Very Unhealthy';
}

// Call the function to fetch air quality data on page load
fetchAirQuality();
