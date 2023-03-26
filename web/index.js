function base36ToBase10(base36Num) {
    return parseInt(base36Num, 36);
}

async function process() {
    let image = new Image();
    image.src = "./levels.bmp";

    const canvas = document.createElement('canvas');
    canvas.width = image.width;
    canvas.height = image.height;

    const context = canvas.getContext('2d');
    console.log({ image });
    context.drawImage(image, 0, 0);

    context.font = 'bold 24px Arial';
    context.fillStyle = '#ffffff';

    context.fillText('Hello, world!', 50, 50);

    document.getElementById('levels').src = canvas.toDataURL();

    // C3.2E73.J9RB
    const data = "C3.2E73.J9RB"

    const parts = data.split(".");

    const levelsData = parts[0]
    const levelsScorePacks = parts.slice(1).reverse();

    const totalLevels = base36ToBase10(levelsData[0]);
    const progress = base36ToBase10(levelsData[1]);

    console.log({
        progress, totalLevels
    })

    let i = 0;
    for (const pack of levelsScorePacks) {
        for (const score of "" + base36ToBase10(pack)) {
            console.log(`level ${i} score -> ${score}`);
            i++;
        }
    }
}
process();
