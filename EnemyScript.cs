using UnityEngine;
using System.Collections;

//unity script for enemy tank behavior
public class EnemyScript : MonoBehaviour
{
    //explosion gameobject
    public GameObject explosion;
    //state for behavior
    int state = 0;
    GameObject state1;
    //object for flag
    Transform flag;
    //number of players
    public static int playernum=1;
    //other tanks
    public GameObject p1;
    public GameObject p2;
    public GameObject p3;
    //target tank
    GameObject Target;
    //distance, rotation in degrees
    Vector3 dis;
    float rot;
    float rot2;
    //range, angle of fire
    float crange;
    float cangle;
    //Just a block you can shoot
    public GameObject Enemy;
    //The first bullet object
    public GameObject bullet1;
    //The second bullet object
    public GameObject bullet2;
    //Use this to control position of second bullet and move them shortly after creation
    GameObject tempb = null;
    //Refers to Cannon
    public GameObject inner;
    //countdown of how long you've been dead
    int deathcd = 0;
    //count for how long to shoot primary cannon
    int reloadtime1 = 0;
    //count of how long to shoot secondary cannons
    int reloadtime2 = 0;
    //your starting location
    Vector3 startloc;
    //Just materials to change color from red to any of the other ones
    public Material blue;
    public Material green;
    public Material yellow;
    //Random integer from 0-4 to determine color
    //int playernum = Mathf.CeilToInt(Random.value * 4F);
    //you aren't dead
    public bool dead = false;
    //Method by which you set your tanks body inactive (dead but ressurectable)
    public void Death()
    {
        //depending on if there are flags, destroy self properly
        if (transform.GetChild(0).transform.GetChild(6).gameObject.activeSelf)
        {
            transform.GetChild(0).transform.GetChild(7).gameObject.SetActive(true);
            transform.GetChild(0).transform.GetChild(7).transform.position = transform.GetChild(0).transform.position + Vector3.down * 2F;
            transform.GetChild(0).transform.GetChild(7).transform.rotation = transform.GetChild(0).transform.rotation;
            transform.GetChild(0).transform.GetChild(7).transform.parent = null;
            transform.GetChild(0).transform.GetChild(6).gameObject.SetActive(false);
        }
        Instantiate(explosion, transform.GetChild(0).position, Quaternion.identity);
        //transform.GetChild(0) refers to the tank's body. The player object actually has both the tank and camera as children
        transform.GetChild(0).gameObject.SetActive(false);
    }
    //Respawn
    public void Revive()
    {
        transform.GetChild(0).gameObject.SetActive(true);
        transform.GetChild(0).position = startloc;
    }
    //Shooting the primary cannon
    void Fire1()
    {
        //transform.GetChild(0).transform.GetChild(3).transform.GetChild(4) is the cannon. This makes a bullet according the the cannon's location and rotation
        Instantiate(bullet1, transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.position, transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.rotation);
    }
    //Shooting the secondary cannons
    void Fire2()
    {
        //These are fired from the body and use the body's location, but using tempb we move them from the body's center to the cannons
        //immediately after their creation
        tempb = (GameObject)Instantiate(bullet2, transform.GetChild(0).transform.position, transform.GetChild(0).transform.rotation);
        tempb.transform.Translate(new Vector3(-3, -.75F, -.75F));
        tempb = (GameObject)Instantiate(bullet2, transform.GetChild(0).transform.position, transform.GetChild(0).transform.rotation);
        tempb.transform.Translate(new Vector3(-3, -.75F, .75F));
    }

    //absolute angle value
    float absangle(float angle)
    {
        if (angle >= 0) { return angle; }
        else { return 360 + angle; }
    }

    //absolute atangent
    float absatan(float a, float b)
    {
        if(a>=0)
        {
            if (b >= 0) { return Mathf.Atan(b / a) * 180 / Mathf.PI; }
            else { return Mathf.Atan(b / a) * 180 / Mathf.PI + 360; }
        }
        else
        {
            return Mathf.Atan(b / a) * 180 / Mathf.PI + 180;
        }
    }

    //sign of what to rotate
    float sign(float a)
    {
        if (a < -180)
        {
            a += 360;
        }
        if (a > 0)
        {
            return 2;
        }
        if (a < 0) { return -2; }
        else { return 0; }
    }

    //negative absolute
    float nabs(float a)
    {
        if (a < -180) { a += 360; }
        return Mathf.Abs(a);
    }

    //find range based on angle
    float truerange(float angle)
    {
        //use algorithm to find how far shell will land
        float t = .001F;
        while ((5 * Mathf.Sin(angle * Mathf.PI / 180) + 1/1.5F) * (1 - Mathf.Exp(-.015F * t)) / .015F - t/1.5F > -2)
        {
            t += .001F;
        }
        t -= .001F;
        return 5 * Mathf.Cos(angle * Mathf.PI / 180) * (1 - Mathf.Exp(-.015F * t)) / .015F;
    }

    void Start() {
        //mark own flag
        flag = transform.GetChild(2);
        transform.GetChild(0).transform.GetChild(6).gameObject.SetActive(false);
        //Color based on tank number
        playernum += 1;
        if (playernum % 4 == 2)
        {
            Material[] mats = transform.GetChild(0).transform.GetChild(1).GetComponent<Renderer>().materials;
            mats[3] = blue;
            transform.GetChild(0).transform.GetChild(1).GetComponent<Renderer>().materials = mats;
            transform.GetChild(0).transform.GetChild(3).transform.GetChild(1).GetComponent<Renderer>().material = blue;
            transform.GetChild(0).transform.GetChild(5).GetComponent<Renderer>().material = blue;
            mats = transform.GetChild(2).transform.GetChild(1).GetComponent<Renderer>().materials;
            mats[0] = blue;
            transform.GetChild(2).transform.GetChild(1).GetComponent<Renderer>().materials = mats;
            mats = transform.GetChild(2).GetChild(3).GetChild(1).GetComponent<Renderer>().materials;
            mats[0] = blue;
            transform.GetChild(2).GetChild(3).GetChild(1).GetComponent<Renderer>().materials = mats;
        }
        if (playernum % 4 == 3)
        {
            Material[] mats = transform.GetChild(0).transform.GetChild(1).GetComponent<Renderer>().materials;
            mats[3] = green;
            transform.GetChild(0).transform.GetChild(1).GetComponent<Renderer>().materials = mats;
            transform.GetChild(0).transform.GetChild(3).transform.GetChild(1).GetComponent<Renderer>().material = green;
            transform.GetChild(0).transform.GetChild(5).GetComponent<Renderer>().material = green;
            mats = transform.GetChild(2).transform.GetChild(1).GetComponent<Renderer>().materials;
            mats[0] = green;
            transform.GetChild(2).transform.GetChild(1).GetComponent<Renderer>().materials = mats;
            mats = transform.GetChild(2).GetChild(3).GetChild(1).GetComponent<Renderer>().materials;
            mats[0] = green;
            transform.GetChild(2).GetChild(3).GetChild(1).GetComponent<Renderer>().materials = mats;
        }
        if (playernum % 4 == 0)
        {
            Material[] mats = transform.GetChild(0).transform.GetChild(1).GetComponent<Renderer>().materials;
            mats[3] = yellow;
            transform.GetChild(0).transform.GetChild(1).GetComponent<Renderer>().materials = mats;
            transform.GetChild(0).transform.GetChild(3).transform.GetChild(1).GetComponent<Renderer>().material = yellow;
            transform.GetChild(0).transform.GetChild(5).GetComponent<Renderer>().material = yellow;
            mats = transform.GetChild(2).transform.GetChild(1).GetComponent<Renderer>().materials;
            mats[0] = yellow;
            transform.GetChild(2).transform.GetChild(1).GetComponent<Renderer>().materials = mats;
            mats = transform.GetChild(2).GetChild(3).GetChild(1).GetComponent<Renderer>().materials;
            mats[0] = yellow;
            transform.GetChild(2).GetChild(3).GetChild(1).GetComponent<Renderer>().materials = mats;
        }
        //Initializes your starting location as where you are respawned
        startloc = transform.GetChild(0).position;
    }

    // Update is called once per frame
    void Update()
    {
        //state0, all tanks alive, choose closest target
        state = 0;
        Target = p1;
        if (p2.transform.GetChild(0).gameObject.activeSelf)
        {
            if (!p1.transform.GetChild(0).gameObject.activeSelf)
            {
                Target = p2;
            }
            else if(Vector3.Magnitude(p2.transform.GetChild(0).position - transform.GetChild(0).position) < Vector3.Magnitude(Target.transform.GetChild(0).position - transform.GetChild(0).position))
            {
                Target = p2;
            }
        }
        if (p3.transform.GetChild(0).gameObject.activeSelf)
        {
            if (!Target.transform.GetChild(0).gameObject.activeSelf)
            {
                Target = p3;
            }
            else if (Vector3.Magnitude(p3.transform.GetChild(0).position - transform.GetChild(0).position) < Vector3.Magnitude(Target.transform.GetChild(0).position - transform.GetChild(0).position))
            {
                Target = p3;
            }
        }
        //state1, one or more tanks are dead, go for their flag
        if(!p1.transform.GetChild(0).gameObject.activeSelf && p1.transform.childCount == 3 || !p2.transform.GetChild(0).gameObject.activeSelf && p2.transform.childCount == 3 || !p3.transform.GetChild(0).gameObject.activeSelf && p3.transform.childCount == 3)
        {
            state = 1;
            state1 = null;
            if(!p1.transform.GetChild(0).gameObject.activeSelf && p1.transform.childCount == 3)
            {
                state1 = p1;
            }
            if (!p2.transform.GetChild(0).gameObject.activeSelf && p2.transform.childCount == 3)
            {
                if (state1==p1)
                {
                    if (Vector3.Magnitude(p2.transform.GetChild(2).position - transform.GetChild(0).position) < Vector3.Magnitude(p1.transform.GetChild(2).position - transform.GetChild(0).position)) {
                        state1 = p2;
                    }
                }
                else
                {
                    state1 = p2;
                }
            }
            if (!p3.transform.GetChild(0).gameObject.activeSelf && p3.transform.childCount == 3)
            {
                if (state1!=null)
                {
                    if (Vector3.Magnitude(p3.transform.GetChild(2).position - transform.GetChild(0).position) < Vector3.Magnitude(state1.transform.GetChild(2).position - transform.GetChild(0).position))
                    {
                        state1 = p3;
                    }
                }
                else
                {
                    state1 = p3;
                }
            }
        }
        //state 2, you have a flag, go home
        if (transform.GetChild(0).GetChild(6).gameObject.activeSelf)
        {
            state = 2;
        }
        //state 3, someone else has your flag, go after them
        if (flag.parent != transform)
        {
            if (!flag.gameObject.activeSelf)
            {
                state = 3;
                Target = flag.parent.parent.gameObject;
            }
            else
            {
                state = 4;
            }
        }
        //Body
        //if you're alive
        if (transform.GetChild(0).gameObject.activeSelf)
        {
            if (Target.transform.GetChild(0).gameObject.activeSelf)
            {
                dis = Target.transform.GetChild(0).position - transform.GetChild(0).position;
                rot = -absangle(transform.GetChild(0).GetChild(3).eulerAngles.y) - absatan(dis.x, dis.z) + 180;
                crange = Vector3.Magnitude(dis);
                cangle = truerange(-transform.GetChild(0).GetChild(3).GetChild(4).eulerAngles.z);
                //targeting
                if (crange < 305 && crange>=120)
                {
                    if (cangle < crange && transform.GetChild(0).GetChild(3).GetChild(4).eulerAngles.z < -18 || cangle > crange && transform.GetChild(0).GetChild(3).GetChild(4).eulerAngles.z > -18)
                    {
                        if (Mathf.Cos(Mathf.PI * transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.eulerAngles.z / 180) < Mathf.Cos(Mathf.PI * (transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.eulerAngles.z + .5F) / 180))
                        {
                            transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.Rotate(Vector3.forward * .1F, Space.Self);
                        }
                    }
                    if (cangle < crange && transform.GetChild(0).GetChild(3).GetChild(4).eulerAngles.z > -18 || cangle > crange && transform.GetChild(0).GetChild(3).GetChild(4).eulerAngles.z < -18)
                    {
                        if (Mathf.Cos(Mathf.PI * transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.eulerAngles.z / 180) > Mathf.Cos(Mathf.PI * (transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.eulerAngles.z - .5F) / 180))
                        {
                            transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.Rotate(Vector3.back * .1F, Space.Self);
                        }
                    }
                }
                if (crange < 120)
                {
                    if (Mathf.Cos(Mathf.PI * transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.eulerAngles.z / 180) < Mathf.Cos(Mathf.PI * (transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.eulerAngles.z + .5F) / 180))
                    {
                        transform.GetChild(0).transform.GetChild(3).transform.GetChild(4).transform.Rotate(Vector3.forward * .1F, Space.Self);
                    }
                }
                //Outside part of Cannon
                transform.GetChild(0).transform.GetChild(3).Rotate(0, sign(rot), 0, Space.World);
                //Cannon
                if (Mathf.Abs(crange-cangle)<20 && crange < 305 && reloadtime1 == 0)
                {
                    Fire1();
                    reloadtime1 = 150;
                }
                /*if (Mathf.Abs(crange - cangle) < 30 && crange < 157.46 && reloadtime1 == 0)
                {
                    Fire1();
                    reloadtime1 = 150;
                }*/
                if (crange < 200 && nabs(rot2) <=3 && reloadtime2 == 0)
                {
                    Fire2();
                    reloadtime2 = 250;
                }
            }
            if (reloadtime1 > 0) { reloadtime1--; }
            if (reloadtime2 > 0) { reloadtime2--; }
            //movement
            switch (state)
            {
                case 0:
                    rot2 = -absangle(transform.GetChild(0).eulerAngles.y) - absatan(dis.x, dis.z) + 180;
                    if (crange < 200)
                    {
                        transform.GetChild(0).transform.Rotate(0, sign(rot2), 0, Space.World);
                    }
                    else
                    {
                        transform.GetChild(0).transform.Rotate(0, sign(rot2 - 45), 0, Space.World);
                    }
                    if (crange > 100)
                    {
                        transform.GetChild(0).transform.Translate(Vector3.left * 0.24F, Space.Self);
                        if (Mathf.Abs(transform.GetChild(0).transform.position.x) > 247 || Mathf.Abs(transform.GetChild(0).transform.position.z) > 247)
                        {
                            transform.GetChild(0).transform.Translate(Vector3.right * 0.24F, Space.Self);
                            transform.GetChild(0).transform.Rotate(0, sign(rot2) * 2F, 0, Space.World);
                        }
                    }
                    else
                    {
                        transform.GetChild(0).transform.Translate(Vector3.left * 0.18F, Space.Self);
                        if (Mathf.Abs(transform.GetChild(0).transform.position.x) > 247 || Mathf.Abs(transform.GetChild(0).transform.position.z) > 247)
                        {
                            transform.GetChild(0).transform.Translate(Vector3.right * 0.18F, Space.Self);
                            transform.GetChild(0).transform.Rotate(0, sign(rot2) * 2F, 0, Space.World);
                        }
                    }
                    break;
                case 1:
                    rot2 = -absangle(transform.GetChild(0).eulerAngles.y) - absatan((state1.transform.GetChild(2).position - transform.GetChild(0).position).x, (state1.transform.GetChild(2).position - transform.GetChild(0).position).z) + 180;
                    transform.GetChild(0).transform.Rotate(0, sign(rot2), 0, Space.World);
                    transform.GetChild(0).transform.Translate(Vector3.left * 0.24F, Space.Self);
                    if (Mathf.Abs(transform.GetChild(0).transform.position.x) > 247 || Mathf.Abs(transform.GetChild(0).transform.position.z) > 247)
                    {
                        transform.GetChild(0).transform.Translate(Vector3.right * 0.24F, Space.Self);
                        transform.GetChild(0).transform.Rotate(0, sign(rot2) * 2F, 0, Space.World);
                    }
                    break;
                case 2:
                    rot2 = -absangle(transform.GetChild(0).eulerAngles.y) - absatan((transform.GetChild(2).position - transform.GetChild(0).position).x, (transform.GetChild(2).position - transform.GetChild(0).position).z) + 180;
                    transform.GetChild(0).transform.Rotate(0, sign(rot2), 0, Space.World);
                    transform.GetChild(0).transform.Translate(Vector3.left * 0.24F, Space.Self);
                    if (Mathf.Abs(transform.GetChild(0).transform.position.x) > 247 || Mathf.Abs(transform.GetChild(0).transform.position.z) > 247)
                    {
                        transform.GetChild(0).transform.Translate(Vector3.right * 0.24F, Space.Self);
                        transform.GetChild(0).transform.Rotate(0, sign(rot2) * 2F, 0, Space.World);
                    }
                    break;
                case 3:
                    if (reloadtime1 > 0) { reloadtime1--; }
                    if (reloadtime2 > 0) { reloadtime2--; }
                    rot2 = -absangle(transform.GetChild(0).eulerAngles.y) - absatan((Target.transform.GetChild(0).position - transform.GetChild(0).position).x, (Target.transform.GetChild(0).position - transform.GetChild(0).position).z) + 180;
                    transform.GetChild(0).transform.Rotate(0, sign(rot2), 0, Space.World);
                    if (crange > 100)
                    {
                        transform.GetChild(0).transform.Translate(Vector3.left * 0.33F, Space.Self);
                        if (Mathf.Abs(transform.GetChild(0).transform.position.x) > 247 || Mathf.Abs(transform.GetChild(0).transform.position.z) > 247)
                        {
                            transform.GetChild(0).transform.Translate(Vector3.right * 0.33F, Space.Self);
                            transform.GetChild(0).transform.Rotate(0, sign(rot2) * 2F, 0, Space.World);
                        }
                    }
                    else
                    {
                        transform.GetChild(0).transform.Translate(Vector3.left * 0.24F, Space.Self);
                        if (Mathf.Abs(transform.GetChild(0).transform.position.x) > 247 || Mathf.Abs(transform.GetChild(0).transform.position.z) > 247)
                        {
                            transform.GetChild(0).transform.Translate(Vector3.right * 0.24F, Space.Self);
                            transform.GetChild(0).transform.Rotate(0, sign(rot2) * 2F, 0, Space.World);
                        }
                    }
                    break;
                case 4:
                    rot2 = -absangle(transform.GetChild(0).eulerAngles.y) - absatan((flag.position - transform.GetChild(0).position).x, (flag.position - transform.GetChild(0).position).z) + 180;
                    transform.GetChild(0).transform.Rotate(0, sign(rot2), 0, Space.World);
                    transform.GetChild(0).transform.Translate(Vector3.left * 0.24F, Space.Self);
                    if (Mathf.Abs(transform.GetChild(0).transform.position.x) > 247 || Mathf.Abs(transform.GetChild(0).transform.position.z) > 247)
                    {
                        transform.GetChild(0).transform.Translate(Vector3.right * 0.24F, Space.Self);
                        transform.GetChild(0).transform.Rotate(0, sign(rot2) * 2F, 0, Space.World);
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            //if you're dead you wait a while and get respawned at your starting location
            deathcd ++;
            if(dead && p2.GetComponent<EnemyScript>().dead && p3.GetComponent<EnemyScript>().dead)
            {
                p1.GetComponent<PlayerScript>().dead=true;
            }
            if ((deathcd >= 1000 || deathcd >= 750 && state == 3) && !dead)
            {
                deathcd = 0;
                Revive();
                transform.GetChild(0).position = startloc;
            }
        }
    }
}
