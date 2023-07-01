function base36ToBase10(base36Num) {
    return parseInt(base36Num, 36);
}

function drawStroked(ctx, text, x, y) {
    ctx.font = '14px monospace';
    ctx.strokeStyle = 'black';
    ctx.lineWidth = 4;
    ctx.strokeText(text, x, y);
    ctx.fillStyle = 'white';
    ctx.fillText(text, x, y);
}

function process(image) {

    const canvas = document.createElement('canvas');
    canvas.width = image.width;
    canvas.height = image.height;

    const context = canvas.getContext('2d');
    context.drawImage(image, 0, 0);

    const data = window.location.pathname.substring(1);

    if (!data) {
        window.location.href = "https://xvayan.itch.io/collie-defense";
        return;
    }

    // C3.2E73.J9RB
    //const data = "C3.2E73.J9RB"

    const parts = data.split(".");

    const levelsData = parts[0]
    const levelsScorePacks = parts.slice(1).reverse();

    const totalLevels = base36ToBase10(levelsData[0]);
    const progress = base36ToBase10(levelsData[1]);

    drawStroked(context, "story progress: " + progress + "/" + totalLevels, 26, 14);

    let i = 0;
    let y = 60;
    for (const pack of levelsScorePacks) {
        for (const score of "" + base36ToBase10(pack)) {
            const x = (i % 2 == 0) ? 64 : 184;
            y = (i % 2 == 1 || i < 2) ? y : y + 80;

            let letter;
            switch (+score) {
                case 2:
                    letter = "D";
                    break;
                case 3:
                    letter = "C";
                    break;
                case 4:
                    letter = "B-";
                    break;
                case 5:
                    letter = "B";
                    break;
                case 6:
                    letter = "B+";
                    break;
                case 7:
                    letter = "A-";
                    break;
                case 8:
                    letter = "A";
                    break;
                case 9:
                    letter = "A+";
                    break;
                case 10:
                    letter = "A++";
                    break;
                default:
                    letter = "-";
                    break;
            }

            drawStroked(context, letter, x, y);

            i++;
        }
    }
    image.src = canvas.toDataURL();
}

let image = document.getElementById('levels');
image.addEventListener(
    "load",
    function () {
        process(image);
    },
    { once: true }
);

let now = new Date();
image.src = "./levels.bmp?ver=" + now.getTime()
